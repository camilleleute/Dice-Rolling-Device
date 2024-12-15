/*
 * UART.c
 *
 *  Created on: Dec 2, 2024
 *      Author: camil
 */
#include "P3.h"

/*
 * @brief: Checks to see if valid inputs were sent in,
 * and sets corresponding values and flags if true
 */
void USART2_IRQHandler(void) {

	if (USART2->ISR & USART_ISR_RXNE) {
		// read keyboard input
		uint16_t input = USART2->RDR;
		switch (input) {
			case '1':
				num_dice = 1;
				break;
			case '2':
				num_dice = 2;
				break;
			case '3':
				num_dice = 3;
				break;
			case 'y':
				roll_again = 1;
				roll_again_flag = 1;
				break;
			case 'n':
				roll_again = 0;
				roll_again_flag = 1;
			default:
				break;
		}
		num_entered_flag = 1;
		// clear flag
		USART2->RQR |= USART_RQR_RXFRQ;
	}

}
/*
 * @brief: Helper function to have variable inputs for moving cursor to make printing easier
 * In form of ESC[A;BH where A is the line number and B is the column number the cursor moves to
 */
void USART_Cursor_Mover(uint8_t line, uint8_t column) {
    UART_print("\033["); // ESC
    // Convert and send the line number
    if (line >= 10) {
        UART_print((char[]){(line / 10) + '0', (line % 10) + '0', '\0'});
    } else {
        UART_print((char[]){line + '0', '\0'});
    }
    // Add the ';'
    UART_print(";");
    // Convert and send the column number
    if (column >= 10) {
        UART_print((char[]){(column / 10) + '0', (column % 10) + '0', '\0'});
    } else {
        UART_print((char[]){column + '0', '\0'});
    }
    // End of the escape sequence
    UART_print("H");
}

/*
 * @brief: Makes printing custom ESC codes simpler
 */
void USART_ESC_Code(char* data) {
    while (!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = '\x1B';  // esc char

    while (*data) {
           while (!(USART2->ISR & USART_ISR_TXE));
           USART2->TDR = *data++;  // Send character and get the next one too
       }
}

/*
 * @brief: Prints message to the UART terminal
 */
void UART_print(char *data) {
	// wait for TXE then write character and advanve till null
    while (*data) {
        while (!(USART2->ISR & USART_ISR_TXE));
        USART2->TDR = *data++;
    }
}


// initializing USART2
void UART_init(void) {
	// enable GPIOA and USART clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// AF7 configuration for PA2 and PA3
	GPIOA->AFR[0] &= ~((0xF << GPIO_AFRL_AFSEL2_Pos)|(0xF << GPIO_AFRL_AFSEL3_Pos)); // clear
	GPIOA->AFR[0] |= ((0x7 << GPIO_AFRL_AFSEL2_Pos)|(0x7 << GPIO_AFRL_AFSEL3_Pos)); // set
	//turn PA2 and PA3 on
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOA->MODER |= (2 << GPIO_MODER_MODE2_Pos) | (2 << GPIO_MODER_MODE3_Pos); // Set to AF mode (0b10)
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);

	// define word length, 8 bits
	USART2->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
	// set baud rate
	USART2->BRR = BRR_num;
	// 1 stop bit
	USART2->CR2 &= ~(USART_CR2_STOP_1 | USART_CR2_STOP_0);
	// enable transmit and receive
	USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE);
	// enable RX interrupts
	USART2->CR1 |= (USART_CR1_RXNEIE);
	//enable USART and interrupt
	USART2->CR1 |= USART_CR1_UE;
	NVIC->ISER[1] |= (1 << (USART2_IRQn & NVIC_MASK));
}


