#include "main.h"
#include "P3.h"

void SystemClock_Config(void);

//global variables
uint8_t num_dice = 0;
uint8_t num_entered_flag = 0;
uint8_t roll_again_flag = 0;
uint8_t roll_again = 0;
uint8_t dice_num = 0;

char time_str[TIME_LEN];
char date_str[DATE_LEN];



int main(void)
{

  __disable_irq();
  HAL_Init();
  SystemClock_Config();
  TIM2_Init();
  UART_init();
  RNG_init();
  RTC_init();
  __enable_irq();


  // clear screen and reset cursor
  USART_ESC_Code("[2J");
  USART_ESC_Code("[H");
  // print start screen
  start_screen();


  while (1)
  {

	  // clear flags
	  roll_again_flag = 0;
	  num_entered_flag = 0;
	  uint8_t valid_num_flag = 0;

	  // if user opts not to roll again or first time rolling, prompt for number of dice
	  if (!(roll_again)) {
	  USART_ESC_Code("[22;5H"); // move cursor line 22 column 5
	  USART_ESC_Code("[0J"); // clear screen from cursor down
	  UART_print("How many die would you like to roll? Enter a number between 1-3");

	  // reset variable that holds number of die wished to be rolled
	  num_dice = 0;

	  // wait for input
	  while (!num_entered_flag);

	  // check if valid input (1, 2, or 3)
	  if ((num_dice <= 3) & (num_dice >= 1)) valid_num_flag = 1;

	  if(!valid_num_flag) {
		  USART_ESC_Code("[24;5H"); // move cursor line 24 column 5
		  UART_print("Please enter a valid input.");
	  }
	  // send msg to player to input a valid answer
	  while (!(valid_num_flag)){
	  		  // poll to see if valid input has been inputed
	  		  if ((num_dice <= 3) & (num_dice >= 1)) valid_num_flag = 1;
	  	}
	  // set roll again to roll select number of die
	  roll_again = 1;
	  }

	  // for rolling, will roll select number of dice or previous number of dice
	  if (roll_again) {
  		  USART_ESC_Code("[24;5H"); // move cursor line 24 column 5
		  USART_ESC_Code("[0J"); // clear screen
		  rolling();

	  }

	  USART_ESC_Code("[36;30H"); // move cursor line 36 column 30
	  USART_ESC_Code("[1m"); // bold
	  USART_ESC_Code("[5m"); // blink
	  UART_print("Roll again? (y/n)");
	  USART_ESC_Code("[0m"); // clear attributes

	  // wait for valid input (y or n) to be inputed
	  while (!roll_again_flag);
  }
}



/*
 * @brief: Rolls select number of dice based on user input. Dice output are determined
 * by random number generator
 */
void rolling(void){
	  uint8_t sum = 0;
	  USART_ESC_Code("[23;5H"); // move cursor line 23 column 5
	  // print number of die being rolled
	  UART_print("Number of dice rolling: ");
	  char dice_rolling_string[] = {num_dice + '0', '\0'};
	  UART_print(dice_rolling_string);

	  // depending on input, roll that many die by using the RNG and sum values
	  if (num_dice >= 1) {
		  dice_num = RNG_polling();
		  dice_printer(1, dice_num);
		  sum += dice_num;
	  }
	  if (num_dice >= 2) {
		  dice_num = RNG_polling();
		  dice_printer(2, dice_num);
		  sum += dice_num;

	  }
	  if (num_dice >= 3) {
		  dice_num = RNG_polling();
		  dice_printer(3, dice_num);
		  sum += dice_num;
	  }
	  USART_ESC_Code("[34;10H"); // move cursor line 34 column 10
	  // print sum of die
	  UART_print("Your dice sum to: ");
	  print_sum(sum);
}

/*
 * @brief: Prints the sum by converting integers to characters
 */
void print_sum(uint8_t sum){
  	// buffer to store char version of sum
	char sum_buf[3];
  	uint8_t idx = 0;

	int temp = sum;
	// Extract each digit
	while (temp > 0) {
		sum_buf[idx++] = '0' + (temp % 10);
		temp /= 10;
	}
  	// Reverse the string
	for (int j = 0; j < idx / 2; j++) {
		char tempChar = sum_buf[j];
		sum_buf[j] = sum_buf[idx - j - 1];
		sum_buf[idx - j - 1] = tempChar;
	}
	// print buffer
	sum_buf[idx] = '\0';
  	UART_print(sum_buf);
  	return;
}



void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
