# lanqiaotest002

> 蓝桥杯嵌入式竞赛练习项目 - STM32G431 开发

![Status](https://img.shields.io/badge/Status-Active-green)
![Platform](https://img.shields.io/badge/Platform-STM32G431-blue)
![Known Issues](https://img.shields.io/badge/Known%20Issues-1%20Fixed-green)

## 📋 项目简介

这是一个基于 STM32G431 的蓝桥杯嵌入式竞赛练习项目，包含 LCD 显示、按键检测等常用功能模块。

## 🛠️ 开发环境

- **开发板**: CT117E (STM32G431)
- **IDE**: Keil MDK-ARM
- **芯片**: STM32G431RBT6
- **语言**: C (96.7%), Assembly (2.8%)

## ⚠️ 重要提醒

### 🚨 LCD 显示异步问题（已解决）

在使用 `LCD_DisplayStringLine()` 等 LCD 显示函数时，**必须特别注意**：

#### ❌ 错误写法（会导致显示异常）
```c
void lcd_show()
{
    char text[20];  // ❌ 局部变量！函数返回后地址失效
    sprintf(text, "count: %d", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

**现象**: LCD 只显示初始值，按键后数字不更新

#### ✅ 正确写法
```c
void lcd_show()
{
    static char text[20];  // ✅ 加 static 关键字
    sprintf(text, "count: %d", count); 
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

#### 📖 详细说明

`LCD_DisplayStringLine()` 是**异步显示函数**，它只保存传入的指针地址，在后台通过中断或 DMA 逐字符显示。如果传递局部变量地址，函数返回后该地址失效，导致 LCD 读取到垃圾数据。

**完整技术文档**: [LCD显示异步问题详解](docs/LCD显示异步问题详解.md)

## 📁 项目结构

```
lanqiaotest002/
├── Core/                   # 核心代码
│   ├── Src/               # 源文件
│   └── Inc/               # 头文件
├── Drivers/               # 驱动库
│   ├── STM32G4xx_HAL_Driver/
│   └── CMSIS/
├── MDK-ARM/               # Keil 项目文件
├── code/                  # 用户代码
├── docs/                  # 项目文档
│   └── LCD显示异步问题详解.md
└── test002.ioc            # STM32CubeMX 配置文件
```

## 🚀 快速开始

1. **克隆项目**
   ```bash
   git clone https://github.com/RtimesC/lanqiaotest002.git
   cd lanqiaotest002
   ```

2. **打开项目**
   - 使用 Keil MDK 打开 `MDK-ARM/` 目录下的项目文件

3. **编译下载**
   - 编译项目（F7）
   - 下载到开发板（F8）

## 📚 文档导航

- [LCD显示异步问题详解](docs/LCD显示异步问题详解.md) - 重要问题排查指南
- [已知问题清单](docs/KNOWN_ISSUES.md) - 所有已知问题汇总

## 🐛 问题反馈

如果你发现任何问题，请：

1. 查看 [已知问题清单](docs/KNOWN_ISSUES.md)
2. 搜索现有 [Issues](https://github.com/RtimesC/lanqiaotest002/issues)
3. 如果是新问题，请 [创建 Issue](https://github.com/RtimesC/lanqiaotest002/issues/new/choose)

## 🏷️ 常见问题标签

- 🐛 `bug` - Bug 问题
- ✨ `enhancement` - 功能增强
- 📝 `documentation` - 文档相关
- 🚨 `critical` - 严重问题
- ❓ `question` - 使用疑问

## 📝 开发注意事项

### 内存安全
- ⚠️ 异步函数（LCD/DMA/中断）不能传递局部变量地址
- ✅ 使用 `static` 局部变量或全局变量
- ✅ 使用 `snprintf` 代替 `sprintf` 防止溢出

### 代码规范
- 函数声明使用 `void func(void)` 而非 `void func()`
- 适当添加 `TODO`、`FIXME` 注释标记

## 📄 许可证

本项目仅用于学习交流，部分驱动代码遵循 Apache License 2.0

---

**作者**: RtimesC  
**最后更新**: 2026-03-08