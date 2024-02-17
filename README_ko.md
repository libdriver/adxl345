[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL345

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl345/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL345는 Analog Devices에서 출시한 작고 얇은 초저전력 3축 가속도계입니다. 최대 ±16g에서 고해상도(13비트) 측정이 가능합니다. 디지털 출력 데이터는 16비트 2의 보수 형식으로 지정되며 SPI(3선 또는 4선) 또는 I2C 디지털 인터페이스를 통해 액세스할 수 있습니다. ADXL345는 모바일 장치 애플리케이션에 적합합니다. 기울기 감지 애플리케이션에서 중력의 정적 가속도와 움직임이나 충격으로 인한 동적 가속도를 측정합니다. 고분해능(3.9 mg/LSB)으로 1.0° 미만의 기울기 변화를 측정할 수 있습니다. 몇 가지 특수 감지 기능이 제공됩니다. 활동 및 비활동 감지는 모든 축의 가속도를 사용자가 설정한 임계값과 비교하여 움직임의 유무를 감지합니다. 탭 감지는 모든 방향의 단일 및 이중 탭을 감지합니다. 자유낙하 감지는 장치가 떨어지는지 감지합니다. 이러한 기능은 두 개의 인터럽트 출력 핀 중 하나에 개별적으로 매핑될 수 있습니다. 32레벨 FIFO(선입 선출) 버퍼가 있는 통합 메모리 관리 시스템은 호스트 프로세서 활동을 최소화하고 전체 시스템 전력 소비를 줄이기 위해 데이터를 저장하는 데 사용할 수 있습니다. .저전력 모드는 임계값 감지 및 극도로 낮은 전력 손실에서 능동 가속 측정을 통해 지능형 동작 기반 전력 관리를 가능하게 합니다.

LibDriver ADXL345는 LibDriver에서 출시한 adxl345의 전체 기능 드라이버입니다. 가속 판독, 가속 FIFO 모드 획득, 자유 낙하 감지, 활동/비활성 상태 감지, 탭 감지 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver ADXL345의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver ADXL345용 플랫폼 독립적인 IIC, SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver ADXL345드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver ADXL345프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver ADXL345오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 ADXL345데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC, SPI 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC, SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

```c
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

### 문서

온라인 문서: [https://www.libdriver.com/docs/adxl345/index.html](https://www.libdriver.com/docs/adxl345/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.