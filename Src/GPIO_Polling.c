#include <stdint.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

#define HIGH        1
#define BTN_PRESSED HIGH

void ConfigGPIOLed1(GPIO_Handle_t *pGPIOLed1); // 配置 PD12 LED (push-pull)
void ConfigGPIOLed2(GPIO_Handle_t *pGPIOLed2); // 配置 PD13 LED (push-pull)
void ConfigGPIOBtn(GPIO_Handle_t *pGPIOBtn); // 配置 PA0 按鈕
void taskLED_blocking();
void taskLED_non_blocking();
void delay(void);

// toggle_LED
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

	//taskLED_blocking();
	taskLED_non_blocking();


	return 0;
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
	pGPIOBtn->GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	pGPIOBtn->GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPD_VHIGH;
	pGPIOBtn->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pGPIOBtn->GPIO_PinConfig.GPIO_PinAltFunMode  = 0;
}

void taskLED_blocking(){
	uint32_t blink_counter = 0; // 用於 LED 閃爍的計數器
	// delay(for loop) -> blocking
	while (1){
		if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == BTN_PRESSED){
			delay();
			GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		}
    // task2 -> 自動閃爍另一個LED
      blink_counter++;
        if (blink_counter >= 1000000) {
            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13); // 切換橙色 LED
            blink_counter = 0; // 重置計數器
      	}
	}
}

void taskLED_non_blocking(){
	// State and Counter -> non-blocking
	uint8_t btn_state = 0;
	uint32_t debounce_count = 0;

	uint32_t blink_counter = 0; // 用於 LED 閃爍的計數器

	while (1) {
	    // 偵測按鍵電位 -> toggle LED (PD12)
	    if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == BTN_PRESSED) {
	        debounce_count++;
	        // 只有當連續偵測到 HIGH 超過一段時間 才視為按下
	        if (debounce_count > 1000 && btn_state == 0) {
	            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
	            btn_state = 1; // 標記已處理，防止按鈕閃太快
	        }
	    } else {
	        debounce_count = 0;
	        btn_state = 0; // 按鍵放開，重置狀態
	    }
	    // task2 -> 自動閃爍另一個LED (PD13)
	        blink_counter++;
	        if (blink_counter >= 1000000) {
	            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13); // 切換橙色 LED
	            blink_counter = 0; // 重置計數器
	        }
	}
}

void delay(void){
	for (volatile uint32_t i = 0; i < 1000000; i++);
}
