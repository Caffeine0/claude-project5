# b站江协科技 STM32 代码风格笔记

> 来源：`Doc/b站江协科技/程序源码/程序源码/STM32Project-有注释版/`
> 之前学 C8T6 时的笔记，代码注释详细，适合参考学习

---

## 一、工程结构

```
STM32Project-有注释版/
├── <例程目录>/
│   ├── User/                 ← 用户代码 (main.c + 中断)
│   │   ├── main.c
│   │   ├── stm32f10x_it.c/h  ← 中断服务函数
│   │   └── stm32f10x_conf.h  ← 外设头文件总开关
│   ├── Library/              ← STM32 标准库驱动 (全部30+外设)
│   │   ├── stm32f10x_gpio.c/h
│   │   ├── stm32f10x_usart.c/h
│   │   ├── stm32f10x_rcc.c/h
│   │   └── ... (完整 StdPeriph V3.5)
│   ├── Start/                ← CMSIS 核心文件
│   │   ├── core_cm3.c/h      ← ARM Cortex-M3 核心
│   │   ├── system_stm32f10x.c/h ← 系统时钟配置
│   │   └── stm32f10x.h       ← 芯片头文件
│   ├── System/               ← 底层函数
│   │   └── Delay.c/h         ← SysTick 延时
│   ├── Project.uvprojx       ← MDK 工程文件
│   └── keilkill.bat          ← 清理脚本
└── STM32Project-无注释版/    ← 同上，代码无注释
```


### 关键结论


---

## 二、代码风格特点

### 1. 注释风格

```c
/*开启时钟*/
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA的时钟
                                                         //使用各个外设前必须开启时钟，否则对外设的操作无效
```

- 块注释说明目的
- 行尾注释说明原因（"为什么"而不是"是什么"）

### 2. 初始化模板（江协风格）

```c
// 1. 开时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);

// 2. 定义结构体
GPIO_InitTypeDef GPIO_InitStructure;

// 3. 配置参数
GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_x;
GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

// 4. 调用初始化
GPIO_Init(GPIOx, &GPIO_InitStructure);
```

这个模板和正点原子一样，可以直接复用。

### 3. GPIO 操作（三种写法他都用了）

| 方法 | 写法 | 推荐 |
|------|------|------|
| GPIO_ResetBits/SetBits | `GPIO_ResetBits(GPIOA, GPIO_Pin_0)` | ✅ 推荐，最清晰 |
| GPIO_WriteBit | `GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET)` | 可用 |
| 强制转换 | `GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0)` | 不推荐，可读性差 |

---

## 三、全部例程目录

| 编号 | 例程 | 涉及外设 | 参考价值 |
|------|------|---------|---------|
| 2-1 | STM32 工程模板 | 无 | 工程搭建参考 |
| 3-1 | LED 闪烁 | GPIO | 基础，已读过 |
| 3-2 | LED 流水灯 | GPIO | 基础 |
| 3-3 | 蜂鸣器 | GPIO | — |
| 3-4 | 按键控制 LED | GPIO+EXTI | 中断触发参考 |
| 3-5 | 光敏传感器控制蜂鸣器 | ADC+GPIO | ADC 参考 |
| 4-1 | OLED 显示屏 | 软件/硬件 I2C + SPI | I2C 参考 |
| 5-1 | 对射式红外传感器计次 | 外部中断 | — |
| 5-2 | 旋转编码器计次 | 定时器编码器模式 | — |
| 6-1 | 定时器定时中断 | TIM | 定时器参考 |
| 6-2~6-8 | 定时器各种模式 | TIM | PWM/捕获/编码器 |
| 7-1~7-2 | AD 单/多通道 | ADC | ADC 参考 |
| 8-1~8-2 | DMA 转运 | DMA+ADC | DMA 参考 |
| 9-1 | 串口发送 | USART | **下一步 USART BSP 直接参考** ✅ |
| 9-2 | 串口发送+接收 | USART+中断 | **直接参考** ✅ |
| 9-3~9-4 | 串口收发数据包 | USART | 协议解析参考 |
| 10-1 | 软件 I2C 读 MPU6050 | 模拟 I2C | I2C 原理参考 |
| 10-2 | 硬件 I2C 读 MPU6050 | 硬件 I2C | **传感器驱动直接参考** ✅ |
| 11-1 | 软件 SPI 读 W25Q64 | 模拟 SPI | SPI 原理参考 |
| 11-2 | 硬件 SPI 读 W25Q64 | 硬件 SPI | W25Q64 参考 |
| 12-1~12-2 | 备份寄存器/RTC | RTC+BKP | — |
| 13-1~13-4 | 主频/睡眠模式 | RCC+低功耗 | 低功耗参考 |
| 14-1~14-2 | 看门狗 | IWDG+WWDG | — |
| 15-1~15-2 | FLASH/芯片ID | FLASH | — |

---

## 四、对我们项目的直接参考价值

| 我们下一步 | 江协参考例程 | 说明 |
|-----------|------------|------|
| USART BSP (USART1+USART3) | `9-1 串口发送` + `9-2 串口发送+接收` | 最直接的参考，中断接收写法可以直接借鉴 |
| ESP8266 AT 指令 | 依赖 USART 成功后再写 | 先跑通串口 |
| MPU6050 驱动 | `10-2 硬件I2C读写MPU6050` | 硬件 I2C 写法，注意 C8T6→ZET6 I2C 引脚 |
| W25Q64 驱动 | `11-2 硬件SPI读写W25Q64` | 霸道板板载 W25Q64 |
| OLED 显示 (可选) | `4-1 OLED显示屏` | 霸道板有 TFT LCD，OLED 是扩展 |

---

## 五、USART 串口例程深度分析（已读）

### Serial.c 核心设计要点

| 功能 | 江协实现 | 我们的 BSP 改进 |
|------|---------|----------------|
| 发送 | `Serial_SendByte` — 查 `USART_FLAG_TXE` | 一致 ✅ |
| 接收 | 中断置标志位，主循环轮询取数 | **环形缓冲区** — 支持多字节缓存 |
| 缓冲区 | 无，单字节 `Serial_RxData` | **2048 字节环形 FIFO** — 防止 AT 响应丢失 |
| printf | `fputc` 重定向到 USART1 | 一致 ✅ |
| NVIC 分组 | 放在 `Serial_Init()` 里 | 放在 `sys.c` 的 `NVIC_Configuration()` — 避免重复 |

### 环形缓冲区设计原因

ESP8266 返回 AT 响应时可能是一次发送多行数据（如 `AT+CWJAP?` 返回 `+CWJAP:"ssid",mac,rssi`），单字节接收会丢数据。环形缓冲区保证中断和 main 循环安全读写。

---

## 六、C8T6 代码直接可用的部分

- **所有标准库 API 调用** — 完全通用，ZET6 兼容
- **Delay 函数** — 直接可用
- **CMSIS + System 文件** — 直接可用
- **外设初始化流程** — 完全一致
- **寄存器操作** — 完全一致

唯一需要改的就是 **GPIO 引脚定义** 和 **外设引脚复用映射**，其余代码几乎不用改。

