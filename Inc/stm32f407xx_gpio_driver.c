/*
 * stm32f407xx_gpio_driver.c
 *
 *
 *      Author: Yin
 */

#include <stdint.h>
#include "stm32f407xx_gpio_driver.h"

/*
 * Peripheral Clock setup
 */

/*************************************************************************************
 * @fn				- GPIO_PeriClockControl
 *
 * @brief           - Enables or disables peripheral clock for the given GPIO port
 *
 * @param[1]        - Base address of the GPIO peripheral
 * @param[2]        - ENABLE or DISABLE macros
 *
 * @return          -none
 *
 * @Not             -none
 *
 */

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDI){

	if (EnorDI == ENABLE){
		if (pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}else if (pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		}else if (pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		}else if (pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		}else if (pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		}else if (pGPIOx == GPIOF){
			GPIOF_PCLK_EN();
		}else if (pGPIOx == GPIOG){
			GPIOG_PCLK_EN();
		}else if (pGPIOx == GPIOI){
			GPIOI_PCLK_EN();
		}
	}else{
		if (pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}else if (pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		}else if (pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		}else if (pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		}else if (pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		}else if (pGPIOx == GPIOF){
			GPIOF_PCLK_DI();
		}else if (pGPIOx == GPIOG){
			GPIOG_PCLK_DI();
		}else if (pGPIOx == GPIOI){
			GPIOI_PCLK_DI();
		}
	}
}


/*
 * Init and De-init
 */

/*************************************************************************************
 * @fn				- GPIO_Init
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -none
 *
 * @Not             -none
 *
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	uint32_t temp = 0; // temp. register

	//enable the peripheral clock

	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// 1. configure the mode of gpio pin

	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		// non interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // 乘以2是因為一個MODER register占2bit，要移動2個bit才能到下一個MODER register的起始位址
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear
		pGPIOHandle->pGPIOx->MODER |= temp; // Set
		temp = 0;
	}else{
		// interrupt mode
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){

			// 1.configure the FTSR
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clear the corresponding RTSR bit
			EXTI->RTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			// 1.configure the RTSR
			EXTI->RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clear the corresponding FTSR bit
			EXTI->FTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			// 1.configure the FTSR and RTSR
			EXTI->RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		// 2. configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);

		// 3. enable the EXTI interrupt delivery using IMR
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	}

	// 2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed = (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear
	pGPIOHandle->pGPIOx->OSPEEDR |= temp; // Set
	temp = 0;

	// 3. configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl = (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear
	pGPIOHandle->pGPIOx->PUPDR |= temp; // Set
	temp = 0;

	// 4. configure the output type
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType = (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clear
	pGPIOHandle->pGPIOx->OTYPER |= temp; // Set
	temp = 0;

	// 5. configure the alternative functionality
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		// configure the alt function register
		uint32_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2)); // Clear
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2)); // Set
	}
}

/*************************************************************************************
 * @fn				- GPIO_Deinit
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -none
 *
 * @Not             -none
 *
 */

void GPIO_Deinit(GPIO_RegDef_t *pGPIOx){
	if (pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}else if (pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}else if (pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	}else if (pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	}else if (pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	}else if (pGPIOx == GPIOF){
		GPIOF_REG_RESET();
	}else if (pGPIOx == GPIOG){
		GPIOG_REG_RESET();
	}else if (pGPIOx == GPIOI){
		GPIOI_REG_RESET();
	}
}

/*
 * Data read and write
 */

/*************************************************************************************
 * @fn				- GPIO_ReadFromInputPin
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          - 0 or 1
 *
 * @Not             -
 *
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

	return value;
}

/*************************************************************************************
 * @fn				- GPIO_ReadFromInputPort
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -
 *
 * @Not             -
 *
 */

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}

/*************************************************************************************
 * @fn				- GPIO_WriteToOutputPin
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -
 *
 * @Not             -
 *
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	if (Value == GPIO_PIN_SET){
		pGPIOx->ODR |= ( 1<< PinNumber );
	}else{
		pGPIOx->ODR &= ~( 1<< PinNumber );
	}
}

/*************************************************************************************
 * @fn				- GPIO_WriteToOutputPort
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -
 *
 * @Not             -
 *
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

/*************************************************************************************
 * @fn				- ToggleOutputPin
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -
 *
 * @Not             -
 *
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= ( 1 << PinNumber );
}

/*
 * IRQ configuration and IRQ handling
 */

/*************************************************************************************
 * @fn				- GPIO_IRQInterruptConfig
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -
 *
 * @Not             -
 *
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDI){

	if (EnorDI == ENABLE){
		if (IRQNumber <= 31){
			// program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );
		}else if (IRQNumber > 31 && IRQNumber < 64){ // 32 to 63
			// program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}else if (IRQNumber >= 64 && IRQNumber < 96);{ // 64 to 95
			// program ISER2 register
			*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
		}
	}else{
		if (IRQNumber <= 31){
			// program ISER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if (IRQNumber > 31 && IRQNumber < 64){ // 32 to 63
			// program ISER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}else if (IRQNumber >= 64 && IRQNumber < 96);{ // 64 to 95
			// program ISER2 register
			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
	    }
	}
}

/*************************************************************************************
* @fn			   - GPIO_IRQPriorityConfig
*
* @brief           -
*
* @param[1]        -
* @param[2]        -
*
* @return          -
*
* @Not             -
*
*/

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	// 1. first lets find out the IPR register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}


/*************************************************************************************
 * @fn				- GPIO_IRQHandling
 *
 * @brief           -
 *
 * @param[1]        -
 * @param[2]        -
 *
 * @return          -
 *
 * @Not             -
 *
 */
void GPIO_IRQHandling(uint8_t PinNumber){
	// clear the EXTI PR register corresponding to the pin number
	if(EXTI->EXTI_PR & ( 1 << PinNumber )){
		//clear
		EXTI->EXTI_PR |= (1 << PinNumber);
	}
}
