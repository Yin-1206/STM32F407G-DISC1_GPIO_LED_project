#include <stdint.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

#define HIGH        1

// 中斷處理函數原型
void EXTI0_IRQHandler(void);

void delay(void);

int main(void){

    GPIO_Handle_t GPIOLed, GPIOBtn;

    // LED 配置 (PD12 - 推挽輸出)
    GPIOLed.pGPIOx = GPIOD;
    GPIOLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_12;
    GPIOLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
    GPIOLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
    GPIOLed.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP;
    GPIOLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    // 按鈕配置 (PA0 - 輸入 + 中斷下降沿)
    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_0;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IT_FT;  // 下降沿中斷
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;          // 內部上拉
    GPIOBtn.GPIO_PinConfig.GPIO_PinAltFunMode  = 0;

    // 初始化
    GPIO_Init(&GPIOLed);
    GPIO_Init(&GPIOBtn);

    // 配置 EXTI0 中斷 (IRQ6)
    GPIO_IRQInterruptConfig(6, ENABLE);      // EXTI0 中斷線 IRQ6
    GPIO_IRQPriorityConfig(6, 0);            // 最高優先級

    // 啟用全域中斷
    __asm("cpsie i");

    while (1){
        // 主迴圈空轉，中斷處理 LED 切換
    }

    return 0;
}

// EXTI0 中斷處理函數 (PA0 按下)
void EXTI0_IRQHandler(void){
    delay();  // 簡單防抖

    // 切換 LED 狀態
    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);

    // 清除中斷標誌
    GPIO_IRQHandling(GPIO_PIN_NO_0);
}

// 延遲函數 (防抖用)
void delay(void){
    for (uint32_t i = 0; i < 50000; i++);
}
