#include <stdint.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

#define HIGH        1

void ConfigGPIOLed1(GPIO_Handle_t *pGPIOLed1); // 配置 PD12 LED (push-pull)
void ConfigGPIOLed2(GPIO_Handle_t *pGPIOLed2); // 配置 PD13 LED (push-pull)
void ConfigGPIOBtn(GPIO_Handle_t *pGPIOBtn); // 配置 PA0 按鈕 (正緣觸發)
void TIM6_Init(void); // 配置TIM6 (PD12 觸發閃爍)
void TIM7_Init(void); // 配置TIM7 (PD13 自動閃爍)
void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);
void EXTI0_IRQHandler(void);

int main(void){

    GPIO_Handle_t GPIOLed1, GPIOLed2, GPIOBtn;

	ConfigGPIOLed1(&GPIOLed1);
	ConfigGPIOLed2(&GPIOLed2);

	ConfigGPIOBtn(&GPIOBtn);

	GPIO_PeriClockControl(GPIOD, ENABLE); // 將GPIO portD 對應bus上clock啟用

	GPIO_Init(&GPIOLed1); // 初始化 PD12 LED
	GPIO_Init(&GPIOLed2); // 初始化 PD13 LED

	GPIO_PeriClockControl(GPIOA, ENABLE); // 將GPIO portA 對應bus上clock啟用

	GPIO_Init(&GPIOBtn); // 初始化 PA0 按鈕

	TIM6_Init(); //初始化 TIM6
	TIM7_Init(); //初始化 TIM7

    // 配置 EXTI0 中斷 (IRQ6)
    GPIO_IRQInterruptConfig(6, ENABLE);      // EXTI0 中斷線 IRQ6
    GPIO_IRQPriorityConfig(6, 0);            // 最高優先級

    // 啟用全域中斷
    __asm("cpsie i");

    while(1){
    	//
    };

    return 0;
}

void TIM6_Init(void) {
    // 1. 啟用 TIM6 時鐘
	TIM6_PCLK_EN();

    // 2. 設定預分頻器 (PSC) -> 產生 1ms 的 tick
    // 預設system clock為 HSI (16MHz)
    TIM6->PSC = 16000 - 1;

    // 3. 設定auto-reload register (ARR) -> 設定為 20ms
    TIM6->ARR = 20 - 1;

    // 4. 啟用更新中斷 (DIER bit 0)
    TIM6->DIER |= (1 << 0);

    // 5. NVIC 配置 (TIM6 IRQ 編號為 54)
    GPIO_IRQInterruptConfig(54, ENABLE);
    GPIO_IRQPriorityConfig(54, 1); // 優先級略低於 EXTI
}

void TIM7_Init(void) {
    TIM7_PCLK_EN(); // 啟用 TIM7 時鐘

    // 設定為 500ms 觸發一次
    TIM7->PSC = 16000 - 1;
    TIM7->ARR = 500 - 1;

    TIM7->DIER |= (1 << 0); // 啟用更新中斷

    // TIM7 的 IRQ 編號為 55
    GPIO_IRQInterruptConfig(55, ENABLE);
    GPIO_IRQPriorityConfig(55, 2); // 優先級設比按鈕觸發低一點

    TIM7->CR1 |= (1 << 0); // 啟動 Timer，讓 LED 開始閃
}

// EXTI0 中斷處理函數 (PA0 按下)
void EXTI0_IRQHandler(void){
	// 檢查 Timer 是否已經在跑了 (避免重複啟動)
	if (!(TIM6->CR1 & (1 << 0))) {
	    // 啟動 TIM6
	    TIM6->CNT = 0;          // 確保從 0 開始
	    TIM6->CR1 |= (1 << 0);
	}

    // 清除中斷標誌
    GPIO_IRQHandling(GPIO_PIN_NO_0);
}

// TIM6 與 DAC 共享中斷向量
void TIM6_DAC_IRQHandler(void) {
    // 1. 檢查並清除中斷標誌位 (UIF: Update Interrupt Flag)
    // 這是狀態暫存器 SR 的第 0 位。如果不清除，CPU 會不斷重複進入此中斷
    if (TIM6->SR & (1 << 0)) {
        TIM6->SR &= ~(1 << 0);
    }

    // 2. 關閉 Timer6 (停止計時)
    // 因為按鍵去彈跳只需要數一次，我們不希望它循環計時
    TIM6->CR1 &= ~(1 << 0);

    // 3. 重置計數器值 (CNT)
    TIM6->CNT = 0;

    // 4. 二次確認按鈕狀態 (Debounce Check)
    if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == HIGH) {
        // 如果 20ms 後仍然是 HIGH，代表這是一次真實的按下動作
        GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12); // 切換 LED 狀態
    }
}

void TIM7_IRQHandler(void) {
    if (TIM7->SR & (1 << 0)) {
        TIM7->SR &= ~(1 << 0); // 清除標誌位
    }

    // 執行閃爍任務
    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13);
}

void ConfigGPIOLed1(GPIO_Handle_t *pGPIOLed1){
	//push-pull (PD12 LED)
	pGPIOLed1->pGPIOx = GPIOD;
	pGPIOLed1->GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_12;
	pGPIOLed1->GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	pGPIOLed1->GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
	pGPIOLed1->GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP;
	pGPIOLed1->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pGPIOLed1->GPIO_PinConfig.GPIO_PinAltFunMode  = 0;

	//open-drain (若使用內部上拉電阻=>電阻太大，會使LED無法閃爍，因此需要外接一個外部電阻)
//	pGPIOLed1->pGPIOx = GPIOD;
//	pGPIOLed1->GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_12;
//	pGPIOLed1->GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
//	pGPIOLed1->GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
//	pGPIOLed1->GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_OD;
//	pGPIOLed1->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
//	pGPIOLed1->GPIO_PinConfig.GPIO_PinAltFunMode  = 0;
}

void ConfigGPIOLed2(GPIO_Handle_t *pGPIOLed2){
	//push-pull (PD13 LED)
	pGPIOLed2->pGPIOx = GPIOD;
	pGPIOLed2->GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13;
	pGPIOLed2->GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	pGPIOLed2->GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
	pGPIOLed2->GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP;
	pGPIOLed2->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pGPIOLed2->GPIO_PinConfig.GPIO_PinAltFunMode  = 0;
}

void ConfigGPIOBtn(GPIO_Handle_t *pGPIOBtn){
	// 按鈕配置
    pGPIOBtn->pGPIOx = GPIOA;
    pGPIOBtn->GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_0;
    pGPIOBtn->GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IT_RT;  // 正緣觸發中斷事件(按下瞬間觸發)
    pGPIOBtn->GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
    pGPIOBtn->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    pGPIOBtn->GPIO_PinConfig.GPIO_PinAltFunMode  = 0;
}

