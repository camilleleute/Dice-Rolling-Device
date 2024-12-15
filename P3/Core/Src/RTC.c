/*
 * RTC.c
 *
 *  Created on: Dec 5, 2024
 *      Author: camil
 */

#include "P3.h"


/*
 * @brief: Initializes RTC to be used as a real time calendar.
 * Initial date and time can be changed in header file.
 */
void RTC_init(void){
	// enable power interface clock
	 RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
	 // enable access to the backup domain
	 PWR->CR1 |= PWR_CR1_DBP;
    // enable power clock
    __HAL_RCC_PWR_CLK_ENABLE();

    // allow access to backup domain
    HAL_PWR_EnableBkUpAccess();

    // enable LSI Oscillator
    RCC->CSR |= RCC_CSR_LSION;

    // wait for LSI to be ready
    while (!(RCC->CSR & RCC_CSR_LSIRDY));

    // select LSI as RTC clock source and reset backup domain
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    RCC->BDCR |= (LSI_SELECT << RCC_BDCR_RTCSEL_Pos);

    // enable RTC clock
    RCC->BDCR |= RCC_BDCR_RTCEN;

    // disable RTC write protection
    RTC->WPR = WP1;
    RTC->WPR = WP2;
	// enter initialization mode
	RTC->ISR |= RTC_ISR_INIT;

	 // wait until initialization is done
	while (!(RTC->ISR & RTC_ISR_INITF));

	// configure the prescalers
	RTC->PRER = (ASYNC_PRE << RTC_PRER_PREDIV_A_Pos) | (SYNC_PRE << RTC_PRER_PREDIV_S_Pos);

	// set the initial time
	RTC->TR = (HOUR_TENS << RTC_TR_HT_Pos) |  // Hour tens
			  (HOUR_ONES << RTC_TR_HU_Pos) | // Hour units
			  (MINUTE_TENS << RTC_TR_MNT_Pos) | // Minute tens
			  (MINUTE_ONES << RTC_TR_MNU_Pos) | // Minute units
			  (SECOND_TENS << RTC_TR_ST_Pos) |  // Second tens
			  (SECOND_ONES << RTC_TR_SU_Pos) |  // Second units
			  (PM << RTC_TR_PM_Pos);   // PM bit set

	// set the initial date
	RTC->DR = (YEAR_TENS << RTC_DR_YT_Pos) |  // Year tens
			  (YEAR_ONES << RTC_DR_YU_Pos) |  // Year units
			  (WEEKDAY << RTC_DR_WDU_Pos) | // Weekday
			  (MONTH_TENS << RTC_DR_MT_Pos) |  // Month tens
			  (MONTH_ONES << RTC_DR_MU_Pos) |  // Month units
			  (DATE_TENS << RTC_DR_DT_Pos) |  // Date tens
			  (DATE_ONES << RTC_DR_DU_Pos);   // Date units

	// 12hr format
	RTC->CR |= RTC_CR_FMT;

	// exit initialization mode
	RTC->ISR &= ~RTC_ISR_INIT;

	// wait to settle after exiting
	while (!(RTC->ISR & RTC_ISR_RSF));

	// enable RTC write protection
	RTC->WPR = WPEN;

	// enable power interface clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
	PWR->CR1 |= PWR_CR1_DBP;

}

/*
 * @brief: Gets the time and date from TR and DR registers,
 * formats and prints them.
 */
void print_time_date(void) {

    // wait for RTC registers to be synchronized
    while (!(RTC->ISR & RTC_ISR_RSF));

    // time register where values are located
    uint32_t time = RTC->TR;
    uint32_t date = RTC->DR;

    // extract BCD values from time register
    uint8_t hour_tens = ((time & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos);
    uint8_t hour_units = ((time & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos);

    // combine BCD vals
    uint8_t hours_bcd = (hour_tens << 4) | hour_units;

    // convert to decimal
    uint8_t hours = B2D(hours_bcd);

    uint8_t min_tens = (time & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos;
    uint8_t min_units = (time & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos;

    // combine BCD vals
    uint8_t min_bcd = ((min_tens << 4) | min_units);

    // convert to decimal
    uint8_t minutes = B2D(min_bcd);

    uint8_t sec_tens = ((time & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos);
    uint8_t sec_units = ((time & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos);

    // combine BCD vals
    uint8_t sec_bcd = (sec_tens << 4) | sec_units;

    // convert to decimal
    uint8_t seconds = B2D(sec_bcd);

    // extract pm or am (0 - am, 1- pm)
    uint8_t pm = (time & RTC_TR_PM_Msk) >> RTC_TR_PM_Pos;


    // convert decimals to printable time string
    sprintf(time_str, "%02d:%02d:%02d %s", hours, minutes, seconds, pm ? "PM" : "AM");
	USART_ESC_Code("[2;20H"); // move cursor line 24 column 5
	USART_ESC_Code("[2K");
	UART_print(time_str);

    // extract BCD values from date register
    uint8_t year_tens = (date & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos;
    uint8_t year_units = (date & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos;
    uint8_t year_bcd = (year_tens << 4) | year_units;
    uint8_t year = B2D(year_bcd);

    uint8_t mon_tens = (date & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos;
    uint8_t mon_units = (date & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos;
    uint8_t mon_bcd = (mon_tens << 4) | mon_units;
    uint8_t month = B2D(mon_bcd);

    uint8_t date_tens = (date & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos;
    uint8_t date_units = (date & RTC_DR_DU_Msk) >> RTC_DR_DU_Pos;
    uint8_t date_bcd = (date_tens << 4) | date_units;
    date_bcd = B2D(date_bcd);

    // convert decimals to printable date string
    sprintf(date_str, "20%02d-%02d-%02d", year, month, date_bcd);
    USART_ESC_Code("[2;40H"); // move cursor line 24 column 5
    UART_print(date_str);

}
/*
 * @brief: converts BCD to decimal
 */
uint8_t B2D(uint8_t bcd) {

	// convert a BCD value to a decimal
    return (bcd & LSBYTE) + ((bcd >> 4) * BCD_MULT);
}


