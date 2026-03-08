# 已知问题清单

> **项目**: lanqiaotest002  
> **最后更新**: 2026-03-08

---

## 🚨 严重问题

### 1. ✅ [已修复] LCD 异步显示局部变量问题

**问题编号**: #1  
**发现日期**: 2026-03-08  
**修复日期**: 2026-03-08  
**严重程度**: 🔴 严重 - 导致功能异常

#### 问题描述

使用 `LCD_DisplayStringLine()` 显示函数时,如果传递局部变量地址,会导致显示内容不更新。

**现象**:
- LCD 只显示初始值 "count: 0"
- 按键改变 count 变量后,显示不更新
- 看起来是静态显示

#### 根本原因

`LCD_DisplayStringLine()` 是异步显示函数,它保存传入的指针地址后立即返回,真正的显示过程在后台通过中断或 DMA 完成。当传递局部变量地址时,函数返回后该变量被销毁,LCD 读取到的是无效内存数据。

#### 错误代码示例

```c
void lcd_show()
{
    char text[20];  // ❌ 局部变量
    sprintf(text, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
    // 函数返回后 text[] 被销毁,但 LCD 还在读取这块内存!
}
```

#### 解决方案

**方案1: 使用 static 局部变量(推荐)**
```c
void lcd_show()
{
    static char text[20];  // ✅ static 使其存储在静态区
    sprintf(text, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

**方案2: 使用全局变量**
```c
char text[20];  // 全局变量

void lcd_show()
{
    sprintf(text, "      count: %d     ", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

**方案3: 为每行使用独立缓冲区(最安全)**
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

#### 影响范围

- ❌ 所有使用 `LCD_DisplayStringLine()` 的代码
- ❌ 所有异步显示/传输函数(DMA、中断驱动)
- ✅ 同步阻塞函数不受影响

#### 详细文档

📖 [LCD显示异步问题详解](LCD显示异步问题详解.md)

#### 预防措施

**代码检查清单**:
- [ ] 异步函数是否传递了局部变量地址?
- [ ] 局部变量是否加了 `static` 关键字?
- [ ] 是否使用 `snprintf` 防止缓冲区溢出?
- [ ] 是否有多个函数共用同一个缓冲区?

---

## ⚠️ 需要注意的问题

### 2. 定时器中断优先级配置

**状态**: 🟡 需要关注  
**发现日期**: 待确认

#### 问题描述

定时器中断和按键中断的优先级设置可能���响按键响应速度。

#### 临时方案

确保按键中断优先级高于定时器中断:
```c
HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);    // 按键中断优先级 0
HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);     // 定时器中断优先级 1
```

---

## 📊 问题统计

| 类别 | 总数 | 已修复 | 进行中 | 待处理 |
|------|------|--------|--------|--------|
| 🚨 严重 | 1 | 1 | 0 | 0 |
| ⚠️ 警告 | 1 | 0 | 0 | 1 |
| 💡 建议 | 0 | 0 | 0 | 0 |
| **总计** | **2** | **1** | **0** | **1** |

---

## 🔍 问题报告指南

如果你发现新问题,请按以下格式报告:

### 报告模板

```markdown
### 问题标题

**状态**: 🔴/🟡/🟢  
**发现日期**: YYYY-MM-DD  
**严重程度**: 严重/警告/建议

#### 问题描述
简洁描述问题

#### 复现步骤
1. 步骤一
2. 步骤二
3. 观察结果

#### 期望行为
应该发生什么

#### 实际行为
实际发生了什么

#### 解决方案
如何修复(如果已知)

#### 影响范围
影响哪些功能
```

---

## 📚 相关文档

- [README.md](../README.md) - 项目主文档
- [LCD显示异步问题详解](LCD显示异步问题详解.md) - 技术深度解析
- [Issues](https://github.com/RtimesC/lanqiaotest002/issues) - GitHub Issues 追踪

---

## 🏷️ 状态图例

| 图标 | 含义 |
|------|------|
| ✅ | 已修复 |
| 🔄 | 进行中 |
| 🔴 | 严重问题 |
| 🟡 | 需要关注 |
| 🟢 | 低优先级 |
| ❌ | 已关闭(无法修复/不是问题)|

---

**维护者**: RtimesC  
**贡献者**: 欢迎提交 PR 更新此文档