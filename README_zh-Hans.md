[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL345

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl345/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL345是亚德诺半导体公司推出的一款小而薄的超低功耗3轴加速度计，分辨率13位，测量范围达±16g。数字输出数据为16位二进制补码格式，可通过SPI(3线或4线)或IIC数字接口访问。ADXL345非常适合移动设备应用。它可以在倾斜检测应用中测量静态重力加速度，还可以测量运动或冲击导致的动态加速度。其高分辨率(3.9 mg/LSB)，能够测量不到1.0°的倾斜角度变化。 该器件提供多种特殊检测功能。活动和非活动检测功能通过比较任意轴上的加速度与用户设置的阈值来检测有无运动发生。敲击检测功能可以检测任意方向的单振和双振动作。自由落体检测功能可以检测器件是否正在掉落。这些功能可以独立映射到两个中断输出引脚中的一个。正在申请专利的集成式存储器管理系统采用一个32级先进先出 (FIFO)缓冲器，可用于存储数据，从而将主机处理器负荷降至最低，并降低整体系统功耗。 低功耗模式支持基于运动的智能电源管理，从而以极低的功耗进行阈值感测和运动加速度测量。该芯片被应用于手机、医疗设备、游戏和定点设备、工业仪器仪表、个人导航设备和硬盘驱动器保护。

LibDriver ADXL345是LibDriver推出的ADXL345的全功能驱动，该驱动提供加速度读取、加速度FIFO模式采集、自由落体检测、运动非运动状态检测、敲击检测等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver ADXL345的源文件。

/interface目录包含了LibDriver ADXL345与平台无关的IIC、SPI总线模板。

/test目录包含了LibDriver ADXL345驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver ADXL345编程范例。

/doc目录包含了LibDriver ADXL345离线文档。

/datasheet目录包含了ADXL345数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC、SPI总线模板，完成指定平台的IIC、SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_adxl345_basic.h"

uint8_t res;
uint8_t i;
float g[3];

res = adxl345_basic_init(ADXL345_INTERFACE_IIC, ADXL345_ADDRESS_ALT_0);                
if (res != 0)
{
    return 1;
}

...
    
for (i = 0; i < 3; i++)
{
    res = adxl345_basic_read((float *)g);
    if (res != 0)
    {
        (void)adxl345_basic_deinit();
        
        return 1;
    }
    adxl345_interface_debug_print("adxl345: x is %0.3f.\n", g[0]);
    adxl345_interface_debug_print("adxl345: y is %0.3f.\n", g[1]);
    adxl345_interface_debug_print("adxl345: z is %0.3f.\n", g[2]);
    adxl345_interface_delay_ms(1000);
    
    ...
        
}

...
    
(void)adxl345_basic_deinit();

return 0;
```

#### example fifo

```C
#include "driver_adxl345_fifo.h"

uint8_t res;
float g[3];

void fifo_callback(float (*g)[3], uint16_t len)
{
    ...
    
    return 0;
}

res = gpio_interrupt_init(adxl345_fifo_irq_handler);
if (res != 0)
{
    return 1;
}
res = adxl345_fifo_init(ADXL345_INTERFACE_IIC, ADXL345_ADDRESS_ALT_0, fifo_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

while (1)
{
    adxl345_interface_delay_ms(10);
    
    ...    
}
...
    
(void)gpio_interrupt_deinit();
(void)adxl345_fifo_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_adxl345_interrupt.h"

uint8_t res;

void interrupt_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL345_INTERRUPT_DATA_READY :
        {
            ...
                
            break;
        }
        case ADXL345_INTERRUPT_SINGLE_TAP :
        {
            adxl345_interface_debug_print("adxl345: irq single tap.\n");
         
            ...
                
            break;
        }
        case ADXL345_INTERRUPT_DOUBLE_TAP :
        {
            adxl345_interface_debug_print("adxl345: irq double tap.\n");
            
            ...
                
            break;
        }
        case ADXL345_INTERRUPT_ACTIVITY :
        {
            adxl345_interface_debug_print("adxl345: irq activity.\n");
            
            ...
                
            break;
        }
        case ADXL345_INTERRUPT_INACTIVITY :
        {
            adxl345_interface_debug_print("adxl345: irq inactivity.\n");
            
            ...
                
            break;
        }
        case ADXL345_INTERRUPT_FREE_FALL :
        {
            adxl345_interface_debug_print("adxl345: irq free fall.\n");
            
            ...
                
            break;
        }
        case ADXL345_INTERRUPT_OVERRUN :
        {
            ...
                
            break;
        }
        default :
        {
            ...
                
            break;
        }
    }
}

res = gpio_interrupt_init(adxl345_interrupt_irq_handler);
if (res != 0)
{
    return 1;
}
res = adxl345_interrupt_init(ADXL345_INTERFACE_SPI, ADXL345_ADDRESS_ALT_0,
                             interrupt_callback,
                             ADXL345_BOOL_TRUE 
                             ADXL345_BOOL_TRUE,
                             ADXL345_BOOL_TRUE,
                             ADXL345_BOOL_TRUE
                            );
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}

...
    
while (1)
{
    if (adxl345_interrupt_server() != 0)
    {
        return 1;
    }
    adxl345_interface_delay_ms(10);
    
    ...
}

...
    
(void)gpio_interrupt_deinit();
(void)adxl345_interrupt_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/adxl345/index.html](https://www.libdriver.com/docs/adxl345/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。