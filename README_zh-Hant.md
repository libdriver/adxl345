[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL345

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl345/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL345是亞德諾電晶體公司推出的一款小而薄的超低功耗3軸加速度計，分辯率13比特，測量範圍達±16g。 數位輸出數據為16比特二進位補數格式，可通過SPI（3線或4線）或IIC數位介面訪問。 ADXL345非常適合移動設備應用。 它可以在傾斜檢測應用中量測靜態重力加速度，還可以量測運動或衝擊導致的動態加速度。 其高解析度（3.9 mg/LSB），能够量測不到1.0°的傾斜角度變化。 該器件提供多種特殊檢測功能。 活動和非活動檢測功能通過比較任意軸上的加速度與用戶設置的閾值來檢測有無運動發生。 敲擊檢測功能可以檢測任意方向的單振和雙振動作。 自由落體檢測功能可以檢測器件是否正在掉落。 這些功能可以獨立映射到兩個中斷輸出引脚中的一個。 正在申請專利的集成式記憶體管理系統採用一個32級先進先出（FIFO）緩衝器，可用於存儲數據，從而將主機處理器負荷降至最低，並降低整體系統功耗。 低功耗模式支持基於運動的智慧電源管理，從而以極低的功耗進行閾值感測和運動加速度量測。 該晶片被應用於手機、醫療設備、遊戲和定點設備、工業儀器儀錶、個人導航設備和硬碟機保護。

LibDriver ADXL345是LibDriver推出的ADXL345的全功能驅動，該驅動提供加速度讀取、加速度FIFO模式採集、自由落體檢測、運動非運動狀態檢測、敲擊檢測等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver ADXL345的源文件。

/interface目錄包含了LibDriver ADXL345與平台無關的IIC、SPI總線模板。

/test目錄包含了LibDriver ADXL345驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver ADXL345編程範例。

/doc目錄包含了LibDriver ADXL345離線文檔。

/datasheet目錄包含了ADXL345數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、SPI總線模板，完成指定平台的IIC、SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/adxl345/index.html](https://www.libdriver.com/docs/adxl345/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。