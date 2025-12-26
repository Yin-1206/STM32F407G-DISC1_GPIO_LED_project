# Project簡介
基於STM32F407G-DISC1開發板，練習實作GPIO driver，透過操作硬體暫存器(Register-level)實現對GPIO的控制，透過輪詢(Polling)與外部中斷(Interrupt)兩種方式實現按下按鈕閃爍LED燈。 

# Project file structure
`Inc` : 存放driver file(stm32f407xx_gpio_driver.h、stm32f407xx_gpio_driver.c)以及stm32f407相關macro(stm32f407xx.h)。

`Src` : 存放main application file
- `main.c`: 透過輪詢(Polling)實現LED閃爍。
- `GPIO_interrupt.c`: 透過外部中斷(Interrupt)實現LED燈閃爍。

# GPIO driver API
## 時脈控制 API
`void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDI)`

`功能` : 啟用/停用 GPIO ports(A~I) 時脈 (AHB1 bus)  

參數 : 
- `GPIO_RegDef_t *pGPIOx` : Base address of GPIO Port. 在`stm32f407xx.h`定義 `GPIOA`、`GPIOB`、`GPIOA`、...、`GPIOI`。
- `uint8_t EnorDI` : Enable or Disable. 在`stm32f407xx.h`定義 `ENABLE`、`DISABLE`。

`實作`：RCC->AHB1ENR 對應位元操作  
><img width="1217" height="518" alt="image" src="https://github.com/user-attachments/assets/112f3ee8-d461-43b5-aca0-266c4d17680a" />

`範例`： GPIO_PeriClockControl(GPIOD, ENABLE);  <啟用 GPIOD 時脈>  
>特別注意: 配置GPIO初始化前，要先啟用對應BUS上的Clock，否則GPIO相關暫存器會失效。 ->此例GPIO port對應AHB1 BUS
><img width="1031" height="756" alt="image" src="https://github.com/user-attachments/assets/fc4df9d1-332b-4d02-b438-b6d47e7e19c9" />
        

## 初始化GPIO API
`void GPIO_Init(GPIO_Handle_t *pGPIOHandle)` 

`功能` : 配置初始化GPIO port pin。  

參數 : 
- `GPIO_Handle_t *pGPIOHandle` : 指向 `Gpio_Handle_t` 配置結構的指標.  

`GPIO_Handle_t` member elements :  
| Data type | Member elements | 目的 |
| :--: | :--: |:--:|
| GPIO_RegDef_t *  | pGPIOx | 設置該Pin所屬之GPIO port的base address. (Ex: GPIOA、GPIOB...)|
| GPIO_PinConfig_t | GPIO_PinConfig | GPIO pin 配置設定 |

`GPIO_PinConfig_t` member elements :
| Data type | Member elements | 目的 |
| :--: | :--: |:--:|
|uint8_t|GPIO_PinNumber|設置要配置的Pin|
|uint8_t|GPIO_PinMode|設置Pin mode (input , output , interrupt , alternate function)|
|uint8_t|GPIO_PinSpeed|設置Pin速度 (low , medium , high , very high)|
|uint8_t|GPIO_PinPuPdControl|設置內部pull up 或 pull down 電阻|
|uint8_t|GPIO_PinOPType|設置 pins output type (push pull/open drain)|
|uint8_t|GPIO_PinAltFunMode|設置 pin for alternate functionality . 若不使用則將此設為0|

## 重置GPIO API
`void GPIO_Deinit(GPIO_RegDef_t *pGPIOx)`  

`功能` : 重置整個 GPIO port 至硬體預設值。

參數 : 
- `GPIO_RegDef_t *pGPIOx` : Base address of GPIO Port. 在`stm32f407xx.h`定義 `GPIOA`、`GPIOB`、`GPIOA`、...、`GPIOI`。

`實作` : RCC->AHB1RSTR 對應位元操作  

><img width="1208" height="503" alt="image" src="https://github.com/user-attachments/assets/0bfef252-8796-456c-bd8e-ac11be5d9e08" />


## 數據讀寫 API
### Read
`uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)`

`功能` : 讀取單一 Pin 輸入電壓 (0/1)。

參數 : 
- `GPIO_RegDef_t *pGPIOx` : Base address of GPIO Port. 在`stm32f407xx.h`定義 `GPIOA`、`GPIOB`、`GPIOA`、...、`GPIOI`。
- `uint8_t PinNumber` : 讀取資料的Pin number. 在`stm32f407xx_gpio_driver.h`定義 `GPIO_PIN_NO_0`、`GPIO_PIN_NO_1`、...、`GPIO_PIN_NO_15`，表示Pin0 ~ Pin15。
><img width="1196" height="517" alt="image" src="https://github.com/user-attachments/assets/ec5c115a-aaba-4109-b062-9cad6b104db7" />


### Write
`void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)`  
`功能` : 讀取單一 Pin 輸出電壓 (0/1)。

參數 :
- `GPIO_RegDef_t *pGPIOx` : Base address of GPIO Port. 在`stm32f407xx.h`定義 `GPIOA`、`GPIOB`、`GPIOA`、...、`GPIOI`。
- `uint8_t PinNumber` : 讀取資料的Pin number. 在`stm32f407xx_gpio_driver.h`定義 `GPIO_PIN_NO_0`、`GPIO_PIN_NO_1`、...、`GPIO_PIN_NO_15`，表示Pin0 ~ Pin15。  
`uint8_t Value` : 寫入GPIO的值/資料。 (可寫入0 或 1，0以外都是為1)
><img width="1197" height="546" alt="image" src="https://github.com/user-attachments/assets/4b31be76-120e-4972-a899-57b114dab0f4" />

### Toggle
`void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)`  

`功能` : 翻轉單一 Pin 輸出狀態 (0 ↔ 1)  

參數 : 
- `GPIO_RegDef_t *pGPIOx` : Base address of GPIO Port. 在`stm32f407xx.h`定義 `GPIOA`、`GPIOB`、`GPIOA`、...、`GPIOI`。
- `uint8_t PinNumber` : 讀取資料的Pin number. 在`stm32f407xx_gpio_driver.h`定義 `GPIO_PIN_NO_0`、`GPIO_PIN_NO_1`、...、`GPIO_PIN_NO_15`，表示Pin0 ~ Pin15。

`運算` : ODR ^= (1 << PinNumber)  <XOR 運算>

# Interrupt相關 API
## 關於STM32微控制器的中斷運作  
根據 Arm Cortex MX 規範，微控制器最多可以支援 240 種不同的中斷。  

這些中斷透過名為 NVIC（巢狀向量中斷控制器）的周邊裝置傳送到處理器內核，NVIC 是 ARM 架構的一部分。對於基於 Arm Cortex M4 架構的 STM32F4 微控制器，在 240 種可用中斷中，STM32F405xx/07xx 和 STM32F415xx/17xx 微控制器僅實現了 82 種中斷。  

所有 GPIO 中斷首先通過 hard wire 的 EXTI peripheral，然後到達 NVIC，最後由 NVIC 向 M4 核心發出中斷訊號。有些 interrupt wire 直接到達 NVIC，而有些則需要通過 EXTI。這是 ST 微控制器特有的，不同廠商的微控制器可能有所不同。

此驅動程式提供了三個 API 來處理 GPIO 中斷，例如在下降沿或上升沿觸發中斷事件、設定優先順序和預設 IRQ 處理程序。

## GPIO_IRQInterruptConfig
`void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDI)`

`功能` : GPIO Pin 的 NVIC interrupt line 啟用or停用

可透過設定`GPIO_PinMode`為`GPIO_MODE_IT_FT`、`GPIO_MODE_IT_RT`、`GPIO_MODE_IT_RFT`使中斷功能生效。  

>特別注意 : 若port D的第14 pin設置為interrupt pin，那麼其他port的第14 pin將不能設置為interrupt pin。

參數 :
- `uint8_t IRQNumber` :

| Pin number | 	IRQ Number | 相關 macros (stm32f407xx.h)|
| :--: | :--: |:--:|
|0|6|`IRQ_NO_EXTI0`|
|1|7|`IRQ_NO_EXTI1`|
|2|8|`IRQ_NO_EXTI2`|
|3|9|`IRQ_NO_EXTI3`|
|4|10|`IRQ_NO_EXTI4`|
|5 ~ 9|23|`IRQ_NO_EXTI9_5`|
|10 ~ 15|40|`IRQ_NO_EXTI15_10`|
- `uint8_t EnorDI` : Enable or Disable. 在`stm32f407xx.h`定義 `ENABLE`、`DISABLE`。
><img width="1172" height="290" alt="image" src="https://github.com/user-attachments/assets/d10830f5-b140-4a8f-bbb0-fe4961956e81" />
><img width="1175" height="216" alt="image" src="https://github.com/user-attachments/assets/a17d8bd5-0c34-4c44-8f69-1083fb387834" />
><img width="1174" height="47" alt="image" src="https://github.com/user-attachments/assets/71525309-8614-4254-a479-5666e7174e23" />
><img width="1172" height="42" alt="image" src="https://github.com/user-attachments/assets/5dccd6b4-8d80-4d95-a54f-d447ef8ea4cf" />

## GPIO_IRQPriorityConfig
`void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)`

`功能` : 設定 NVIC 中斷優先級 (0=最高)

參數 :
- `uint8_t IRQNumber` :

| Pin number | 	IRQ Number | 相關 macros (stm32f407xx.h)|
| :--: | :--: |:--:|
| 0 |6|`IRQ_NO_EXTI0`|
|1|7|`IRQ_NO_EXTI1`|
|2|8|`IRQ_NO_EXTI2`|
|3|9|`IRQ_NO_EXTI3`|
|4|10|`IRQ_NO_EXTI4`|
|5 ~ 9|23|`IRQ_NO_EXTI9_5`|
|10 ~ 15|40|`IRQ_NO_EXTI15_10`|
- `uint32_t IRQPriority` : IRQ 優先級 (範圍: 0 ~ 15)
><img width="897" height="626" alt="image" src="https://github.com/user-attachments/assets/80fd7e0d-0951-4e7f-b738-e0118c8f1b53" />
><img width="901" height="346" alt="image" src="https://github.com/user-attachments/assets/b77b703e-cb4c-4e6b-8dad-8303e9e98d45" />


## GPIO_IRQHandling
`void GPIO_IRQHandling(uint8_t PinNumber)`

`功能` : 清除 EXTI 中斷標誌 (寫1清零)

`關鍵` : EXTI->PR[PinNumber] = 1  
><img width="1189" height="622" alt="image" src="https://github.com/user-attachments/assets/464d8255-12ad-4c6a-a85b-7932cff94330" />


>特別注意 : 必須在 ISR 最後執行，否則中斷重複觸發

參數 : 
- `uint8_t PinNumber` : 讀取資料的Pin number. 在`stm32f407xx_gpio_driver.h`定義 `GPIO_PIN_NO_0`、`GPIO_PIN_NO_1`、...、`GPIO_PIN_NO_15`，表示Pin0 ~ Pin15。
