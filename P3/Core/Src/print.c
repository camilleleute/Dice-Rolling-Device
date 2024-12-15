/*
 * print.c
 *
 *  Created on: Dec 2, 2024
 *      Author: camil
 */


/*
 * printing_funcs.c
 *
 *  Created on: Dec 8, 2024
 *      Author: camil
 */
#include "P3.h"
/*
 * @brief: Prints the header on the page, the word DICE and a 3D beneath it
 */
void start_screen(void) {
	USART_ESC_Code("[3;15H"); // move cursor line 3 column 20
	UART_print("      ____      _______    _____    ______");
	USART_ESC_Code("[4;15H"); // move cursor line 3 column 20
	UART_print("     / _  \\    /__  __/|  / ___/|  / ____/| ");
	USART_ESC_Code("[5;15H"); // move cursor line 3 column 20
	UART_print("    / / | |    |_/ /|_|/ / /|__|/ / /___ |/");
	USART_ESC_Code("[6;15H"); // move cursor line 3 column 20
	UART_print("   / / / /|     / / /   / / /    / ____/|");
	USART_ESC_Code("[7;15H"); // move cursor line 3 column 20
	UART_print("  / /_/ / / __ / /_/   / /_/_   / /____|/");
	USART_ESC_Code("[8;15H"); // move cursor line 3 column 20
	UART_print(" /_____/ / /_______/|  |____/| /______/|");
	USART_ESC_Code("[9;15H"); // move cursor line 3 column 20
	UART_print(" |_____|/  |_______|/  |____|/ |______|/");
	USART_ESC_Code("[10;15H"); // move cursor line 3 column 20

   // print dice in good location
	USART_ESC_Code("[12;28H"); // move cursor line 3 column 20
    UART_print("   ________");
    USART_ESC_Code("[13;28H"); // move cursor line 3 column 20
    UART_print("  /\\       \\ ");
    USART_ESC_Code("[14;28H"); // move cursor line 3 column 20
    UART_print(" /❤ \\   ❤   \\ ");
    USART_ESC_Code("[15;28H"); // move cursor line 3 column 20
    UART_print("/    \\_______\\ ");
    USART_ESC_Code("[16;28H"); // move cursor line 3 column 20
    UART_print("\\    /❤      / ");
    USART_ESC_Code("[17;28H"); // move cursor line 3 column 20
	UART_print(" \\❤ /   ❤   / ");
	USART_ESC_Code("[18;28H"); // move cursor line 3 column 20
	UART_print("  \\/______❤/");
	USART_ESC_Code("[20;5H"); // move cursor line 3 column 20
	UART_print("_________________________________________________________________");
}

/*
 * @brief: Prints corresponding dice image and in the correct place
 */
void dice_printer(uint8_t num, uint8_t dice_num){
	uint8_t column;
	if (num == 1) {
		column = DIE_ONE_PLACEMENT;
	}
	else if (num == 2) {
		column = DIE_TWO_PLACEMENT;
	}
	else if (num == 3) {
		column = DIE_THREE_PLACEMENT;
	}
	switch(dice_num){
		case(1):
			dice_one(column);
			break;
		case(2):
			dice_two(column);
			break;
		case(3):
			dice_three(column);
			break;
		case(4):
			dice_four(column);
			break;
		case(5):
			dice_five(column);
			break;
		case(6):
			dice_six(column);
			break;
		default:
			break;
	}
}

/*
 * @brief: Prints image of the one-side of a dice and value rolled
 */
void dice_one (uint8_t column){
	uint8_t line = STARTING_LINE;
	USART_Cursor_Mover(line++, column);
	UART_print("  _________");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |    ❤    |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |_________|");
	line++;
	USART_Cursor_Mover(line, column);
	UART_print("You rolled a one!");
}


/*
 * @brief: Prints image of the two-side of a dice and value rolled
 */
void dice_two(uint8_t column) {
	uint8_t line = STARTING_LINE;
	USART_Cursor_Mover(line++, column);
	UART_print("  _________");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤      |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |      ❤  |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |_________|");
	line++;
	USART_Cursor_Mover(line++, column);
	UART_print("You rolled a two!");

}

/*
 * @brief: Prints image of the three-side of a dice and value rolled
 */
void dice_three(uint8_t column) {
	uint8_t line = STARTING_LINE;
	USART_Cursor_Mover(line++, column);
	UART_print("  _________");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤      |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |    ❤    |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |      ❤  |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |_________|");
	line++;
	USART_Cursor_Mover(line++, column);
	UART_print("You rolled a three!");

}

/*
 * @brief: Prints image of the four-side of a dice and value rolled
 */
void dice_four(uint8_t column) {
	uint8_t line = STARTING_LINE;
	USART_Cursor_Mover(line++, column);
	UART_print("  _________");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |_________|");
	line++;
	USART_Cursor_Mover(line++, column);
	UART_print("You rolled a four!");
}

/*
 * @brief: Prints image of the five-side of a dice and value rolled
 */
void dice_five(uint8_t column) {
	uint8_t line = STARTING_LINE;
	USART_Cursor_Mover(line++, column);
	UART_print("  _________");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |    ❤    |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |_________|");
	line++;
	USART_Cursor_Mover(line++, column);
	UART_print("You rolled a five!");
}


/*
 * @brief: Prints image of the six-side of a dice and value rolled
 */
void dice_six(uint8_t column) {
	uint8_t line = STARTING_LINE;
	USART_Cursor_Mover(line++, column);
	UART_print("  _________");
	USART_Cursor_Mover(line++, column);
	UART_print(" |         |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |  ❤  ❤   |");
	USART_Cursor_Mover(line++, column);
	UART_print(" |_________|");
	line++;
	USART_Cursor_Mover(line++, column);
	UART_print("You rolled a six!");
}

