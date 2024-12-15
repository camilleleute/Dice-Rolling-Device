/*
 * RNG.c
 *
 *  Created on: Dec 3, 2024
 *      Author: camil
 */

#include "P3.h"

/*
 * @brief: Polls for RNG by waiting for flags to give the OK
 * before loading in random number from data register
 */
uint8_t RNG_polling(void) {
    // Enable RNG
    RNG->CR |= RNG_CR_RNGEN;
    // Wait for no seed or clock errors
    while (RNG->SR & (RNG_SR_SEIS | RNG_SR_CEIS)) {
    	// re-enable RNG if error
        RNG->CR &= ~RNG_CR_RNGEN;
        RNG->CR |= RNG_CR_RNGEN;
    }
    // Wait for the data ready flag
    while (!(RNG->SR & RNG_SR_DRDY));
    // Read from the data register
    uint32_t dice_num = RNG->DR;
    // mask it so its within range
    return (dice_num % MASK) + MASK_OFFSET;
}

/*
 * @brief: Initializes the RNG by setting the correct clocks and bits
 */
void RNG_init(void){
	// set RNG clock to 48MHz to generate analog noise to create random number
	PLLQ_CONFIG_48MHz();

	// Select PLLQ as the RNG clock source
	RCC->CCIPR &= ~RCC_CCIPR_CLK48SEL; // Clear CLK48SEL bits
	RCC->CCIPR |= (PLLQ << RCC_CCIPR_CLK48SEL_Pos); // Select PLLQ
	// enable RNG clock
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
	// disable interrupts
	RNG->CR &= ~RNG_CR_IE;
}
/*
 * @brief: Sets PLL clock to 48Mhz by using the 16MHz HSI clock as a base
 */
void PLLQ_CONFIG_48MHz(void) {
    // enable HSI and wait for it to stabilize
    if (!(RCC->CR & RCC_CR_HSION)) {
        RCC->CR |= RCC_CR_HSION;
        while (!(RCC->CR & RCC_CR_HSIRDY));
    }

    // turn off the PLL if it is currently on and wait for it to stabilize
    if (RCC->CR & RCC_CR_PLLON) {
        RCC->CR &= ~RCC_CR_PLLON;
        while (RCC->CR & RCC_CR_PLLRDY);
    }

    // configure PLL source and factors for 48 MHz on PLLQ
    // HSI = 16 MHz, PLLM = 4, PLLN = 48, PLLQ = 4
    RCC->PLLCFGR = 0; // reset
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;        // Select HSI as PLL source
    RCC->PLLCFGR |= (HSI_DIVIDER << RCC_PLLCFGR_PLLM_Pos);   // Divide HSI by 4 (16 MHz / 4 = 4 MHz)
    RCC->PLLCFGR |= (HSI_MULTIPLIER << RCC_PLLCFGR_PLLN_Pos);  // Multiply by 48 (4 MHz * 48 = 192 MHz)
    RCC->PLLCFGR |= (HSI_DIVIDER << RCC_PLLCFGR_PLLQ_Pos);   // Divide by 4 for PLLQ output (192 MHz / 4 = 48 MHz)
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN;            // Enable PLLQ output

    // enable the PLL and wait for it to lock
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
}

