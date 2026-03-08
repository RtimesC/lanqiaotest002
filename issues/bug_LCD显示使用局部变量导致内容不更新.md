## 🐛 Bug 描述

使用 `LCD_DisplayStringLine()` 函数显示内容时,如果传递局部变量的地址,会导致LCD显示内容无法更新。

## 📋 复现步骤

1. 编写如下代码:
```c
void lcd_show()
{
    char text[20];  // 局部变量
    sprintf(text, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

2. 在主循环中调用 `lcd_show()` 函数
3. 通过按键改变 `count` 变量的值
4. 观察LCD显示

## ✅ 期望行为

LCD应该显示更新后的 count 值,例如:
- 初始: "count: 0"
- 按键后: "count: 1", "count: 2", ...

## ❌ 实际行为

LCD只显示初始值 "count: 0",无论如何改变 count 变量,显示内容都不更新。

## 🔍 根本原因

`LCD_DisplayStringLine()` 是**异步显示函数**:

1. 该函数只保存传入的指针地址
2. 函数立即返回
3. 真正的显示过程在后台通过中断或DMA完成
4. 当传递局部变量地址时,函数返回后该变量被销毁
5. LCD后台读取时,地址指向的内存已经无效

### 内存生命周期问题

```
时间线:
0ms:  调用 lcd_show()
      └─ char text[20] 在栈上创建
1ms:  函数返回
      └─ text[] 被销毁,栈空间可被覆盖 ⚠️
2ms:  LCD中断读取 text[0]  ❌ 野指针!
3ms:  LCD中断读取 text[1]  ❌ 垃圾数据
...
```

## 💡 解决方案

### 方案1: 使用 static 局部变量 (推荐)

```c
void lcd_show()
{
    static char text[20];  // ✅ 加 static 关键字
    sprintf(text, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

**优��:**
- ✅ 变量存储在静态区,生命周期延长
- ✅ 作用域仅限函数内部,封装性好
- ✅ 地址固定,异步显示安全

### 方案2: 使用全局变量

```c
char text[20];  // 全局变量

void lcd_show()
{
    sprintf(text, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

### 方案3: 为每行使用独立缓冲区 (最安全)

```c
void lcd_show()
{
    static char line0_buf[20];
    static char line3_buf[20];
    
    sprintf(line0_buf, "        test        "); 
    LCD_DisplayStringLine(Line0, (uint8_t *)line0_buf);
    
    sprintf(line3_buf, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)line3_buf);
}
```

## 💻 环境信息

- **开发板**: CT117E
- **芯片型号**: STM32G431RBT6
- **IDE/编译器**: Keil MDK
- **问题类型**: 内存生命周期问题

## 📚 影响范围

此问题影响所有异步函数:

- ❌ `LCD_DisplayStringLine()` - LCD显示
- ❌ `HAL_UART_Transmit_DMA()` - DMA串口发送
- ❌ 所有中断驱动的异步函数
- ✅ 同步阻塞函数不受影响

## 🔗 相关文档

- 📖 [LCD显示异步问题详解](docs/LCD显示异步问题详解.md) - 详细技术文档
- 📋 [已知问题清单](docs/KNOWN_ISSUES.md) - 问题汇总

## ⚠️ 预防措施

**代码检查清单:**
- [ ] 异步函数是否传递了局部变量地址?
- [ ] 局部变量是否加了 `static` 关键字?
- [ ] 是否使用 `snprintf` 防止缓冲区溢出?
- [ ] 是否有多个函数共用同一个缓冲区?

## 📝 状态

- **发现日期**: 2026-03-08
- **修复日期**: 2026-03-08
- **状态**: ✅ 已修复
- **解决方案**: 使用 static 局部变量

---

**关键提醒**: 这是一个非常隐蔽但很常见的bug,容易被忽视。所有使用异步函数的代码都需要检查!