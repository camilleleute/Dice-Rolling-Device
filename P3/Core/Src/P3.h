/*
 * P3.h
 *
 *  Created on: Dec 2, 2024
 *      Author: camil
 */

#ifndef SRC_P3_H_
#define SRC_P3_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx_hal.h"

#define WP1 0xCA
#define WP2 0x53
#define WPEN 0xFF

#define LSI_SELECT 2
#define ASYNC_PRE 127
// maybe 255
#define SYNC_PRE 249
#define TIME_LEN 15
#define DATE_LEN 15
#define LSBYTE 0xF
#define BCD_MULT 10
#define ONESECOND 1000
#define MICROSEC_PRESC 3999

#define PLLQ 2
#define MASK 6
#define MASK_OFFSET 1
#define HSI_DIVIDER 4
#define HSI_MULTIPLIER 48
#define NVIC_MASK 0x1F
#define DIE_ONE_PLACEMENT 10
#define DIE_TWO_PLACEMENT 30
#define DIE_THREE_PLACEMENT 50
#define STARTING_LINE 25

// change to current time
#define HOUR_TENS 1
#define HOUR_ONES 2
#define MINUTE_TENS 0
#define MINUTE_ONES 0
#define SECOND_TENS 0
#define SECOND_ONES 0
#define PM 1


// change to current date
#define YEAR_TENS 2
#define YEAR_ONES 4
#define WEEKDAY 1
#define MONTH_TENS 1
#define MONTH_ONES 2
#define DATE_TENS 0
#define DATE_ONES 9
#define BRR_num 35

extern uint8_t num_dice;
extern uint8_t num_entered_flag;
extern uint8_t roll_again_flag;
extern uint8_t roll_again;
extern uint8_t dice_num;
extern char time_str[];
extern char date_str[];



void USART_Cursor_Mover(uint8_t, uint8_t);
void USART_ESC_Code(char*);
void UART_print(char *);
void UART_init(void);
void print_time_date(void);
uint8_t B2D(uint8_t);
void PLLQ_CONFIG_48MHz(void);
void rolling(void);
void print_sum(uint8_t);
void start_screen(void);
void dice_printer(uint8_t, uint8_t);
void dice_one (uint8_t);
void dice_two(uint8_t);
void dice_three(uint8_t);
void dice_four(uint8_t);
void dice_five(uint8_t);
void dice_six(uint8_t);

uint8_t RNG_polling(void);
void RNG_init(void);

void RTC_init(void);

void TIM2_Init(void);

#endif /* SRC_P3_H_ */
