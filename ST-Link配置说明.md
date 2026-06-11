# ST-Link 下载配置说明

## 概述

本文档说明如何配置 Keil 项目使用 ST-Link 进行下载，并启用 Reset and Run 功能。

---

## 一、工程配置修改

已完成以下配置：

### 1. 目标芯片
- **设备型号**: `STM32F103ZE` (从 C8T6 迁移)
- **Flash 大小**: 512KB
- **RAM 大小**: 64KB

### 2. 调试器配置
- **调试器**: ST-Link (ST-LINKIII-KEIL_SWO.DLL)
- **Flash 算法**: ST-LINKIII-KEIL_SWO.DLL

### 3. 编译宏
- `STM32F10X_HD` (从 MD 改为 HD)
- `USE_STDPERIPH_DRIVER`

### 4. 启动文件
- `startup_stm32f10x_hd.s` (从 md 改为 hd)

---

## 二、手动设置 Reset and Run (重要!)

### 操作步骤

1. **打开 Keil 项目**
   ```
   打开 Project\C8T6 Proj.uvprojx
   ```

2. **进入调试设置**
   ```
   Project -> Options for Target (Alt+F7)
   -> Debug 选项卡
   -> 点击 "Settings" 按钮
   ```

3. **配置 Flash Download**
   ```
   在弹出的 "Target Driver Setup" 窗口中:
   -> 选择 "Flash Download" 选项卡
   -> 勾选 "Reset and Run" 复选框
   -> 确保 "Erase Full Chip" (可选)
   ```

4. **检查 ST-Link 连接**
   ```
   -> 选择 "Debug" 选项卡
   -> 确保 "ST-Link Debugger" 已选中
   -> 点击 "OK" 确认设置
   ```

---

## 三、使用下载脚本

### 方法 1: 使用 download.bat (推荐)

1. **连接硬件**
   ```
   - ST-Link V2/V3 连接电脑
   - ST-Link 连接开发板 (SWCLK, SWDIO, GND)
   - 开发板通电
   ```

2. **运行脚本**
   ```
   双击运行: download.bat
   
   脚本会自动:
   1. 重新编译项目
   2. 下载程序到 Flash
   3. 自动复位运行
   ```

### 方法 2: 手动下载

1. **编译项目**
   ```
   Keil: Build Target (F7)
   或运行: build.bat
   ```

2. **下载程序**
   ```
   Keil: Download (F8)
   ```

3. **复位运行**
   ```
   Keil: Reset (Ctrl+Shift+F2)
         或: Run (F5)
   ```

---

## 四、验证下载成功

### 串口输出检查

下载成功后，观察串口调试助手：

**正常输出示例**:
```
 Usart init OK
 0. AT
 Send cmd: AT
 Send cmd: AT
 ...
```

**如果看到 "0. AT" 重复输出**:
- 说明 ESP8266 模块未响应
- 检查 ESP8266 硬件连接
- 检查 USART3 TX/RX 是否正确

---

## 五、常见问题

### 问题 1: ST-Link 未识别

**解决方法**:
1. 检查 ST-Link USB 驱动是否安装
2. 在设备管理器中查看是否有 "ST-Link Debug" 设备
3. 安装 ST-Link 驱动: `D:\studySoft\KEIL\ARM\STLink\ST-LinkUpgrade.exe`

### 问题 2: 下载失败

**检查项**:
1. 目标芯片型号是否为 STM32F103ZE
2. SWD 接口是否连接正确 (SWCLK, SWDIO, GND, 3.3V)
3. 芯片是否被加密/锁定

### 问题 3: 串口无输出

**检查项**:
1. 串口连接是否正确 (TX->RX, RX->TX)
2. 波特率是否为 115200
3. 串口参数: 8-N-1

---

## 六、文件清单

| 文件 | 说明 |
|------|------|
| `build.bat` | 编译脚本 |
| `build.ps1` | PowerShell 编译脚本 |
| `download.bat` | 下载并运行脚本 |
| `Project\C8T6 Proj.uvprojx` | Keil 工程文件 |
| `Output\C8T6 Proj.hex` | 编译输出的 HEX 文件 |
| `Output\build_log.txt` | 编译日志 |

---

## 七、下一步

1. ✅ 已完成: 代码迁移 (C8T6 -> ZET6)
2. ✅ 已完成: 编译验证 (0 Error)
3. ⏳ 待完成: 下载测试
4. ⏳ 待完成: 功能验证

---

**日期**: 2026-06-11
**状态**: 配置完成，待硬件测试