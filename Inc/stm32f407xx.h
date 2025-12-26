/*
 * stm32f407xx.h
 *
 *
 *      Author: Yin
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include <stddef.h>

#define __weak __attribute__((weak))

/*
 * ARM Cortex M4 Processor NVIC ISERx register address
 */

#define NVIC_ISER0		((volatile uint32_t *)0xE000E100)
#define NVIC_ISER1		((volatile uint32_t *)0xE000E104)
#define NVIC_ISER2		((volatile uint32_t *)0xE000E108)
#define NVIC_ISER3		((volatile uint32_t *)0xE000E10C)
#define NVIC_ISER4		((volatile uint32_t *)0xE000E110)
#define NVIC_ISER5		((volatile uint32_t *)0xE000E114)
#define NVIC_ISER6		((volatile uint32_t *)0xE000E118)
#define NVIC_ISER7		((volatile uint32_t *)0xE000E11C)


/*
 * ARM Cortex M4 Processor NVIC ICERx register address
 */

#define NVIC_ICER0		((volatile uint32_t *)0xE000E180)
#define NVIC_ICER1		((volatile uint32_t *)0xE000E184)
#define NVIC_ICER2		((volatile uint32_t *)0xE000E188)
#define NVIC_ICER3		((volatile uint32_t *)0xE000E18C)
#define NVIC_ICER4		((volatile uint32_t *)0xE000E190)
#define NVIC_ICER5		((volatile uint32_t *)0xE000E194)
#define NVIC_ICER6		((volatile uint32_t *)0xE000E198)
#define NVIC_ICER7		((volatile uint32_t *)0xE000E19C)


/*
 * ARM Cortex M4 Processor NVIC IPRx register address
 */

#define NVIC_PR_BASE_ADDR		((volatile uint32_t *)0xE000E400)


#define NO_PR_BITS_IMPLEMENTED  4

/*
 * Flash、SRAM、ROM base addresses
 */

#define FLASH_BASEADDR 	              0x08000000U
#define SRAM1_BASEADDR                0x20000000U    //SRAM1共佔112 KB = 112*1024 B = 1 C000(HEX)
#define SRAM2_BASEADDR	              0x2001C000U
#define ROM	                          0x1FFF0000U    //system memory
#define SRAM                          SRAM1_BASEADDR //SRAM1為Main SRAM


/*
 * ABPx and AHBx bus peripheral base addresses
 */

#define PERIPHERAL_BASEADDR           0x40000000U
#define ABP1PERIPHERAL_BASEADDR       PERIPHERAL_BASEADDR
#define ABP2PERIPHERAL_BASEADDR       0x40010000U
#define AHB1PERIPHERAL_BASEADDR 	  0x40020000U
#define AHB2PERIPHERAL_BASEADDR       0x50000000U
#define AHB3PERIPHERAL_BASEADDR       0xA0000000U


/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */

#define GPIOA_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x0000)
#define GPIOB_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x0400)
#define GPIOC_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x0800)
#define GPIOD_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x1000)
#define GPIOF_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x1400)
#define GPIOG_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x1800)
#define GPIOH_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR                (AHB1PERIPHERAL_BASEADDR + 0x2000)

#define RCC_BASEADDR				  (AHB1PERIPHERAL_BASEADDR + 0x3800)


/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */

#define I2C1_BASEADDR                 (ABP1PERIPHERAL_BASEADDR + 0x5400)
#define I2C2_BASEADDR                 (ABP1PERIPHERAL_BASEADDR + 0x5800)
#define I2C3_BASEADDR                 (ABP1PERIPHERAL_BASEADDR + 0x5C00)

#define SPI2_BASEADDR                 (ABP1PERIPHERAL_BASEADDR + 0x3800)
#define SPI3_BASEADDR                 (ABP1PERIPHERAL_BASEADDR + 0x3C00)

#define USART2_BASEADDR               (ABP1PERIPHERAL_BASEADDR + 0x4400)
#define USART3_BASEADDR               (ABP1PERIPHERAL_BASEADDR + 0x4800)
#define UART4_BASEADDR                (ABP1PERIPHERAL_BASEADDR + 0x4C00)
#define UART5_BASEADDR                (ABP1PERIPHERAL_BASEADDR + 0x5000)


/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */


#define SPI1_BASEADDR                 (ABP2PERIPHERAL_BASEADDR + 0x3000)
#define SPI4_BASEADDR                 (ABP2PERIPHERAL_BASEADDR + 0x3400)
#define USART1_BASEADDR               (ABP2PERIPHERAL_BASEADDR + 0x1000)
#define USART6_BASEADDR               (ABP2PERIPHERAL_BASEADDR + 0x1400)
#define EXTI_BASEADDR                 (ABP2PERIPHERAL_BASEADDR + 0x3C00)
#define SYSCFG_BASEADDR               (ABP2PERIPHERAL_BASEADDR + 0x3800)


/*****************************peripheral register definition structures****************************/

typedef struct{
	volatile uint32_t MODER;        	//GPIO port mode register                   address offset: 0x00
	volatile uint32_t OTYPER;       	//GPIO port output type register            address offset: 0x04
	volatile uint32_t OSPEEDR;      	//GPIO port output speed register           address offset: 0x08
	volatile uint32_t PUPDR;        	//GPIO port pull-up/pull-down register      address offset: 0x0C
	volatile uint32_t IDR;		    	//GPIO port input data register             address offset: 0x10
	volatile uint32_t ODR;		    	//GPIO port output data register            address offset: 0x14
	volatile uint32_t BSRR;		    	//GPIO port bit set/reset register          address offset: 0x18
	volatile uint32_t LCKR;		    	//GPIO port configuration lock register     address offset: 0x1C
	volatile uint32_t AFR[2];		    //GPIO alternate function low register      address offset: 0x20~0x24

}GPIO_RegDef_t;

/*
 * peripheral register definition structure for RCC
 */

typedef struct{
	volatile uint32_t RCC_CR;       	//RCC clock control register                address offset: 0x00
	volatile uint32_t RCC_PLLCFGR;  	//RCC PLL configuration register            address offset: 0x04
	volatile uint32_t RCC_CFGR;     	//RCC clock configuration register          address offset: 0x08
	volatile uint32_t RCC_CIR;      	//RCC clock interrupt register              address offset: 0x0C
	volatile uint32_t RCC_AHB1RSTR; 	//RCC AHB1 peripheral reset register        address offset: 0x10
	volatile uint32_t RCC_AHB2RSTR;		//RCC AHB2 peripheral reset register        address offset: 0x14
	volatile uint32_t RCC_AHB3RSTR;		//RCC AHB3 peripheral reset register        address offset: 0x18
	uint32_t RCC_Reserved1;	//                                                      address offset: 0x1C
	volatile uint32_t RCC_APB1RSTR;		//APB1 peripheral reset register            address offset: 0x20
	volatile uint32_t RCC_APB2RSTR;		//APB2 peripheral reset register            address offset: 0x24
	uint32_t RCC_Reserved2;	//                                                      address offset: 0x28
	uint32_t RCC_Reserved3;	//                                                      address offset: 0x2C
	volatile uint32_t RCC_AHB1ENR;	    //RCC AHB1 peripheral clock enable register address offset: 0x30
	volatile uint32_t RCC_AHB2ENR;	    //RCC AHB2 peripheral clock enable register address offset: 0x34
	volatile uint32_t RCC_AHB3ENR;	    //RCC AHB3 peripheral clock enable register address offset: 0x38
	uint32_t RCC_Reserved4;	//                                                      address offset: 0x3C
	volatile uint32_t RCC_APB1ENR;	    //RCC APB1 peripheral clock enable register address offset: 0x40
	volatile uint32_t RCC_APB2ENR;	    //RCC APB2 peripheral clock enable register address offset: 0x44
	uint32_t RCC_Reserved5;	//                                                      address offset: 0x48
	uint32_t RCC_Reserved6;	//                                                      address offset: 0x4C
	volatile uint32_t RCC_AHB1LPENR;	//RCC AHB1 peripheral clock enable in low power mode register     address offset: 0x50
	volatile uint32_t RCC_AHB2LPENR;	//RCC AHB1 peripheral clock enable in low power mode register     address offset: 0x54
	volatile uint32_t RCC_AHB3LPENR;	//RCC AHB1 peripheral clock enable in low power mode register     address offset: 0x58
	uint32_t RCC_Reserved7;	//                                                      address offset: 0x5C
	volatile uint32_t RCC_APB1LPENR;	//RCC APB1 peripheral clock enable in low power mode register     address offset: 0x60
	volatile uint32_t RCC_APB2LPENR;	//RCC APB1 peripheral clock enable in low power mode register     address offset: 0x64
	uint32_t RCC_Reserved8;	//                                                      address offset: 0x68
	uint32_t RCC_Reserved9;	//                                                      address offset: 0x6C
	volatile uint32_t RCC_BDCR;	        //RCC Backup domain control register        address offset: 0x70
	volatile uint32_t RCC_CSR;	        //RCC clock control & status register       address offset: 0x74
	uint32_t RCC_Reserved10;	//                                                  address offset: 0x78
	uint32_t RCC_Reserved11;	//                                                  address offset: 0x7C
	volatile uint32_t RCC_SSCGR;	    //RCC spread spectrum clock generation register     address offset: 0x80
	volatile uint32_t RCC_PLLI2SCFGR;	//RCC PLLI2S configuration register         address offset: 0x84
}RCC_RegDef_t;

/*
 * peripheral register definition structure for SPI
 */
typedef struct{
    volatile uint32_t CR1;        // SPI Control register 1,                  @ offset 0x00
    volatile uint32_t CR2;        // SPI Control register 2,                  @ offset 0x04
    volatile uint32_t SR;         // SPI Status register 1,                   @ offset 0x08
    volatile uint32_t DR;         // SPI data register,                       @ offset 0x0C
    volatile uint32_t CRCPR;      // SPI CRC polynomial register,             @ offset 0x10
    volatile uint32_t RXCRCR;     // SPI Rx CRC register,                     @ offset 0x14
    volatile uint32_t TXCRCR;     // SPI Tx CRC register,                     @ offset 0x18
    volatile uint32_t I2SCFGR;    // SPI_I2S configuration register,          @ offset 0x1C
    volatile uint32_t I2SPR;      // SPI_I2S prescaler register,              @ offset 0x20
} SPI_RegDef_t;


/*
 * peripheral register definition structure for I2C
 */
typedef struct{
    volatile uint32_t CR1;        // @ offset 0x00
    volatile uint32_t CR2;        // @ offset 0x04
    volatile uint32_t OAR1;       // @ offset 0x08
    volatile uint32_t OAR2;       // @ offset 0x0C
    volatile uint32_t DR;         // @ offset 0x10
    volatile uint32_t SR1;        // @ offset 0x14
    volatile uint32_t SR2;        // @ offset 0x18
    volatile uint32_t CCR;        // @ offset 0x1C
    volatile uint32_t TRISE;      // @ offset 0x20
    volatile uint32_t FLTR;       // @ offset 0x24
} I2C_RegDef_t;


/*
 * peripheral register definition structure for EXTI
 */

typedef struct{
	volatile uint32_t IMR;       	// address offset: 0x00
	volatile uint32_t EMR;       	// address offset: 0x04
	volatile uint32_t RTSR;      	// address offset: 0x08
	volatile uint32_t FTSR;      	// address offset: 0x0C
	volatile uint32_t EXTI_SWIER;	// address offset: 0x10
	volatile uint32_t EXTI_PR;	    // address offset: 0x14
}EXTI_RegDef_t;


/*
 * peripheral register definition structure for SYSCFG
 */

typedef struct{
	volatile uint32_t MEMRMP;           // address offset: 0x00
	volatile uint32_t PMC;              // address offset: 0x04
	volatile uint32_t EXTICR[4];        // address offset: 0x08~0x14
	volatile uint32_t Reserved1;        // address offset: 0x18
	volatile uint32_t Reserved2;        // address offset: 0x1C
	volatile uint32_t CMPCR;            // address offset: 0x20
}SYSCFG_RegDef_t;

/*
 * peripheral definitions (base address)
 */

#define GPIOA  ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB  ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC  ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD  ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE  ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF  ((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG  ((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH  ((GPIO_RegDef_t *)GPIOH_BASEADDR)
#define GPIOI  ((GPIO_RegDef_t *)GPIOI_BASEADDR)

#define SPI1  ((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2  ((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3  ((SPI_RegDef_t *)SPI3_BASEADDR)
#define SPI4  ((SPI_RegDef_t *)SPI4_BASEADDR)

#define I2C1  ((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2  ((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3  ((I2C_RegDef_t *)I2C3_BASEADDR)



#define RCC    ((RCC_RegDef_t *)RCC_BASEADDR)

#define EXTI   ((EXTI_RegDef_t *)EXTI_BASEADDR)

#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)


/*
 * Clock enable macros for GPIOx peripheral
 */

#define GPIOA_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 0 ))
#define GPIOB_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 1 ))
#define GPIOC_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 2 ))
#define GPIOD_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 3 ))
#define GPIOE_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 4 ))
#define GPIOF_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 5 ))
#define GPIOG_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 6 ))
#define GPIOH_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 7 ))
#define GPIOI_PCLK_EN()    (RCC->RCC_AHB1ENR |= ( 1 << 8 ))

/*
 * Clock enable macros for I2Cx peripheral
 */

#define I2C1_PCLK_EN()     (RCC->RCC_APB1ENR |= ( 1 << 21 ))
#define I2C2_PCLK_EN()     (RCC->RCC_APB1ENR |= ( 1 << 22 ))
#define I2C3_PCLK_EN()     (RCC->RCC_APB1ENR |= ( 1 << 23 ))

/*
 * Clock enable macros for SPIx peripheral
 */

#define SPI1_PCLK_EN()     (RCC->RCC_APB2ENR |= ( 1 << 12 ))
#define SPI2_PCLK_EN()     (RCC->RCC_APB1ENR |= ( 1 << 14 ))
#define SPI3_PCLK_EN()     (RCC->RCC_APB1ENR |= ( 1 << 15 ))
#define SPI4_PCLK_EN()     (RCC->RCC_APB2ENR |= ( 1 << 13 ))

/*
 * Clock enable macros for USARTx peripheral
 */

#define USART1_PCLK_EN()   (RCC->RCC_APB2ENR |= ( 1 << 4 ))
#define USART2_PCLK_EN()   (RCC->RCC_APB1ENR |= ( 1 << 17 ))
#define USART3_PCLK_EN()   (RCC->RCC_APB1ENR |= ( 1 << 18 ))
#define UART34_PCLK_EN()   (RCC->RCC_APB1ENR |= ( 1 << 19 ))
#define UART35_PCLK_EN()   (RCC->RCC_APB1ENR |= ( 1 << 20 ))
#define USART6_PCLK_EN()   (RCC->RCC_APB2ENR |= ( 1 << 5 ))

/*
 * Clock enable macros for SYSCFG peripheral
 */

#define SYSCFG_PCLK_EN()   (RCC->RCC_APB2ENR |= ( 1 << 14 ))

/*
 * Clock disable macros for GPIOx peripheral
 */

#define GPIOA_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 0 ))
#define GPIOB_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 1 ))
#define GPIOC_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 2 ))
#define GPIOD_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 3 ))
#define GPIOE_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 4 ))
#define GPIOF_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 5 ))
#define GPIOG_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 6 ))
#define GPIOH_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 7 ))
#define GPIOI_PCLK_DI()    (RCC->RCC_AHB1ENR &= ~( 1 << 8 ))


/*
 * Clock disable macros for I2Cx peripheral
 */

#define I2C1_PCLK_DI()     (RCC->RCC_APB1ENR &= ~( 1 << 21 ))
#define I2C2_PCLK_DI()     (RCC->RCC_APB1ENR &= ~( 1 << 22 ))
#define I2C3_PCLK_DI()     (RCC->RCC_APB1ENR &= ~( 1 << 23 ))


/*
 * Clock disable macros for SPIx peripheral
 */

#define SPI1_PCLK_DI()     (RCC->RCC_APB2ENR &= ~( 1 << 12 ))
#define SPI2_PCLK_DI()     (RCC->RCC_APB1ENR &= ~( 1 << 14 ))
#define SPI3_PCLK_DI()     (RCC->RCC_APB1ENR &= ~( 1 << 15 ))
#define SPI4_PCLK_DI()     (RCC->RCC_APB2ENR &= ~( 1 << 13 ))



/*
 * Clock disable macros for USARTx peripheral
 */

#define USART1_PCLK_DI()   (RCC->RCC_APB2ENR &= ~( 1 << 4 ))
#define USART2_PCLK_DI()   (RCC->RCC_APB1ENR &= ~( 1 << 17 ))
#define USART3_PCLK_DI()   (RCC->RCC_APB1ENR &= ~( 1 << 18 ))
#define UART34_PCLK_DI()   (RCC->RCC_APB1ENR &= ~( 1 << 19 ))
#define UART35_PCLK_DI()   (RCC->RCC_APB1ENR &= ~( 1 << 20 ))
#define USART6_PCLK_DI()   (RCC->RCC_APB2ENR &= ~( 1 << 5 ))


/*
 * Clock disable macros for SYSCFG peripheral
 */

#define SYSCFG_PCLK_DI()   (RCC->RCC_APB2ENR &= ~( 1 << 14 ))


/*
 * Macros to reset GPIOx peripherals
 */

#define GPIOA_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 0 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 0 )); }while(0)
#define GPIOB_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 1 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 1 )); }while(0)
#define GPIOC_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 2 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 2 )); }while(0)
#define GPIOD_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 3 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 3 )); }while(0)
#define GPIOE_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 4 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 4 )); }while(0)
#define GPIOF_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 5 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 5 )); }while(0)
#define GPIOG_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 6 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 6 )); }while(0)
#define GPIOH_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 7 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 7 )); }while(0)
#define GPIOI_REG_RESET()          do{ (RCC->RCC_AHB1RSTR |= ( 1 << 8 )); (RCC->RCC_AHB1RSTR &= ~( 1 << 8 )); }while(0)

/*
 * returns port code for given GPIOx base address
 */

#define GPIO_BASEADDR_TO_CODE(x)  ( (x == GPIOA) ? 0 : \
		                            (x == GPIOB) ? 1 : \
                                    (x == GPIOC) ? 2 : \
        		                    (x == GPIOD) ? 3 : \
        		                    (x == GPIOE) ? 4 : \
        		                    (x == GPIOF) ? 5 : \
        				            (x == GPIOG) ? 6 : \
        						    (x == GPIOH) ? 7 : \
        		                    (x == GPIOI) ? 8 : 0 )

/*
 * IRQ number of STM32F407x MCU
 */

#define IRQ_NO_EXTI0      6
#define IRQ_NO_EXTI1      7
#define IRQ_NO_EXTI2      8
#define IRQ_NO_EXTI3      9
#define IRQ_NO_EXTI4      10
#define IRQ_NO_EXTI9_5    23
#define IRQ_NO_EXTI15_10  40
#define IRQ_NO_SPI1       35
#define IRQ_NO_SPI2       36
#define IRQ_NO_SPI3       51
#define IRQ_NO_I2C1_EV    31
#define IRQ_NO_I2C1_ER    32
/*
 * macros for all the possible priority levels
 */

#define NVIC_IRQ_PRIO0    0
#define NVIC_IRQ_PRIO1    1
#define NVIC_IRQ_PRIO2    2
#define NVIC_IRQ_PRIO3    3
#define NVIC_IRQ_PRIO4    4
#define NVIC_IRQ_PRIO5    5
#define NVIC_IRQ_PRIO6    6
#define NVIC_IRQ_PRIO7    7
#define NVIC_IRQ_PRIO8    8
#define NVIC_IRQ_PRIO9    9
#define NVIC_IRQ_PRIO10   10
#define NVIC_IRQ_PRIO11   11
#define NVIC_IRQ_PRIO12   12
#define NVIC_IRQ_PRIO13   13
#define NVIC_IRQ_PRIO14   14
#define NVIC_IRQ_PRIO15   15

// Some generic macros

#define ENABLE          1
#define DISABLE         0
#define SET             ENABLE
#define RESET           DISABLE
#define GPIO_PIN_SET    SET
#define GPIO_PIN_RESET  RESET
#define FLAG_SET        SET
#define FLAG_RESET      RESET



#include "stm32f407xx_gpio_driver.h"

#endif /* INC_STM32F407XX_H_ */
