#include <stdint.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

#define HIGH        1

void ConfigGPIOLed1(GPIO_Handle_t *pGPIOLed1); // 配置 PD12 LED (push-pull)
void ConfigGPIOLed2(GPIO_Handle_t *pGPIOLed2); // 配置 PD13 LED (push-pull)
void ConfigGPIOBtn(GPIO_Handle_t *pGPIOBtn); // 配置 PA0 按鈕 (正緣觸發)
void EXTI0_IRQHandler(void);
void delay(void);

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

    // 配置 EXTI0 中斷 (IRQ6)
    GPIO_IRQInterruptConfig(6, ENABLE);      // EXTI0 中斷線 IRQ6
    GPIO_IRQPriorityConfig(6, 0);            // 最高優先級

    // 啟用全域中斷
    __asm("cpsie i");

    uint32_t blink_counter = 0;

    while (1){
    	// 主迴圈負責 PD13 的閃爍任務
        blink_counter++;
    	if (blink_counter >= 1000000) {
    	    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13);
    	    blink_counter = 0;
        }
    }

    return 0;
}

// EXTI0 中斷處理函數 (PA0 按下)
void EXTI0_IRQHandler(void){
    delay();

    // 切換 LED 狀態
    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);

    // 清除中斷標誌
    GPIO_IRQHandling(GPIO_PIN_NO_0);
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

void delay(void){
    for (volatile uint32_t i = 0; i < 250000; i++);
}
