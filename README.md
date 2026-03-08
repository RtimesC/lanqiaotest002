# 蓝桥杯嵌入式竞赛项目 - lanqiaotest002

[![Status](https://img.shields.io/badge/Status-Active-success)](https://github.com/RtimesC/lanqiaotest002)
[![Known Issues](https://img.shields.io/badge/Known%20Issues-1-orange)](https://github.com/RtimesC/lanqiaotest002/issues)
[![MCU](https://img.shields.io/badge/MCU-STM32G431-blue)](https://www.st.com/en/microcontrollers-microprocessors/stm32g4-series.html)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

> 蓝桥杯嵌入式开发板 CT117E 测试项目

---

## 📋 项目简介

本项目基于 STM32G431 芯片，用于蓝桥杯嵌入式竞赛的功能测试和开发。

### 硬件平台
- **开发板**: CT117E-M (蓝桥杯官方开发板)
- **主控芯片**: STM32G431RBT6
- **开发环境**: Keil MDK-ARM
- **调试接口**: ST-Link V2

---

## ⚠️ 重要提醒

### 🚨 LCD 显示关键问题

在使用 LCD 显示功能时，**务必注意以下问题**，否则会导致显示异常：

```c
// ❌ 错误写法 - 会导致显示不更新
void lcd_show()
{
    char text[20];  // 局部变量！函数返回后内存失效
    sprintf(text, "count: %d", count);
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}

// ✅ 正确写法 - 使用 static 关键字
void lcd_show()
{
    static char text[20];  // static 使变量存储在静态区
    sprintf(text, "count: %d", count);
    LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
```

**原因**: `LCD_DisplayStringLine()` 是异步函数，会在后台中断中逐字符显示。如果传递局部变量地址，函数返回后该内存会被释放，导致 LCD 读取到垃圾数据。

📖 **详细说明**: [LCD显示异步问题详解](docs/LCD显示异步问题详解.md)  
🐛 **相关Issue**: [#1 LCD显示局部变量导致内容不更新](https://github.com/RtimesC/lanqiaotest002/issues/1)

---

## 🗂️ 项目结构

```
lanqiaotest002/
├── Core/                    # STM32 核心代码
│   ├── Inc/                # 头文件
│   └── Src/                # 源文件
│       └── main.c          # 主程序
├── Drivers/                # STM32 驱动库
│   ├── CMSIS/              # CMSIS 库
│   └── STM32G4xx_HAL_Driver/  # HAL 库
├── MDK-ARM/                # Keil 工程文件
├── code/                   # 自定义代码
├── docs/                   # 项目文档
│   ├── LCD显示异步问题详解.md
│   └── KNOWN_ISSUES.md     # 已知问题清单
├── .mxproject              # STM32CubeMX 配置
└── test002.ioc             # CubeMX 项目文件
```

---

## 🚀 快速开始

### 1. 克隆项目

```bash
git clone https://github.com/RtimesC/lanqiaotest002.git
cd lanqiaotest002
```

### 2. 打开工程

使用 Keil MDK 打开 `MDK-ARM/test002.uvprojx`

### 3. 编译下载

1. 编译项目 (F7)
2. 连接开发板
3. 下载程序 (F8)

---

## 📚 功能模块

- [x] LCD 显示控制
- [x] 按键输入检测
- [x] 定时器配置
- [ ] ADC 采集
- [ ] PWM 输出
- [ ] UART 通信

---

## 📖 文档导航

| 文档 | 说明 |
|------|------|
| [LCD显示异步问题详解](docs/LCD显示异步问题详解.md) | LCD 显示局部变量问题完整分析 |
| [已知问题清单](docs/KNOWN_ISSUES.md) | 项目已知问题汇总 |

---

## 🐛 已知问题

查看完整问题清单: [KNOWN_ISSUES.md](docs/KNOWN_ISSUES.md)

### 严重问题

- **LCD显示异常** ✅ 已修复 - 使用 static 局部变量替代普通局部变量
  - 详见: [Issue #1](https://github.com/RtimesC/lanqiaotest002/issues/1)
  - 文档: [LCD显示异步问题详解](docs/LCD显示异步问题详解.md)

---

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

### 提交 Issue

如果你发现了问题，请：
1. 查看 [已知问题清单](docs/KNOWN_ISSUES.md)
2. 如果是新问题，创建 [新 Issue](https://github.com/RtimesC/lanqiaotest002/issues/new)

### 代码规范

- 使用 `static` 声明 LCD 显示缓冲区
- 使用 `snprintf` 代替 `sprintf` 防止溢出
- 添加必要的注释说明

---

## 📊 开发记录

### v1.1.0 (2026-03-08)
- ✅ 修复 LCD 显示局部变量问题
- 📝 添加详细技术文档
- 🏷️ 创建问题追踪系统

### v1.0.0 (初始版本)
- ⚙️ 基础工程搭建
- 📟 LCD 显示功能实现
- 🔘 按键检测功能

---

## 📞 联系方式

- **作者**: RtimesC
- **GitHub**: [@RtimesC](https://github.com/RtimesC)
- **项目**: [lanqiaotest002](https://github.com/RtimesC/lanqiaotest002)

---

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

---

## 🙏 致谢

- STMicroelectronics - STM32 HAL 库
- 蓝桥杯组委会 - 开发板支持

---

**⭐ 如果这个项目对你有帮助，请给一个 Star！**