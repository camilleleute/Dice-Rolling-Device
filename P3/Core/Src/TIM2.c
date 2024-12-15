/*
 * TIM2.c
 *
 *  Created on: Dec 5, 2024
 *      Author: camil
 */
#include "P3.h"

/*
* @brief Initializes TIM2
*/
void TIM2_Init(void){
	// enable TIM2 clock
	RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM2EN);
	// set TIM2 as an up counter
	TIM2->CR1 &= ~TIM_CR1_DIR;
	// Pre-scaler value to make each count equal to 1 ms
	TIM2->PSC = MICROSEC_PRESC;
   	// enter interrupt every second
   	TIM2->ARR = ONESECOND;
	// enable update event interrupt in TIM2 and CCR1 interrupts
	TIM2->DIER |=(TIM_DIER_UIE);
	// clear interrupt status register for update event and CCR1
	TIM2->SR &= ~(TIM_SR_UIF);
	// start timer
	TIM2->CR1 |= TIM_CR1_CEN;
	// enable TIM2 interrupts
	NVIC->ISER[0] = (1 <<(TIM2_IRQn & NVIC_MASK));
	// Enable timer
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void){

	if (TIM2->SR & TIM_SR_UIF) {
		// update time and date every second
		print_time_date();
		// clear update event interrupt flag
		TIM2->SR &= ~(TIM_SR_UIF);
	}
}

