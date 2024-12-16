# P3: Final Design Project, Dice Rolling Device

## Project Behavior
The device is a user interactive dice rolling machine that tells time. The user is able to select from one to three for how many dice they would like to roll. After rolling, an image of the face of the die, the number, and the sum of all die rolled is displayed. The real time and date is displayed on screen to keep track of how long a game lasts.

## System Specifications
Microcontroller ______________________________________________________ STM32L476
System Clock Frequency ____________________________________________________ 4Mhz
Terminal Baud Rate __________________________________________________ 115,200kbps
Terminal ________________________________________________________________ VT100
Number of Dice to Roll ___________________________________________________ 1, 2, or 3
Possible Numbers to Roll _______________________________________________ 1 through 6
Time Display Format _____________________________________________ 12 hr HH:MM:SS
Date Display Format ____________________________________________ year - month - date

## Software Architecture
### Code Overview
The way the program works is that on start, the start screen is displayed asking the user how many dice they would like to roll. If a number besides 1,2, or 3 is pressed, the device reminds the user to input a valid number. The program polls for the flag to see if there was a valid keypress. Once a valid number is selected, the program then uses the random number generator to decide what number the dice will be. Since the transmission register outputs a 32 bit random number, the final value must be masked and offset to be in range. Then the associated die image and number are displayed. This repeats for the number of dice rolled. At the end, the sum of the dice is displayed to make in-game calculations quick. The user is then prompted on whether they would like to roll again or not. ‘y’ is the input for yes, and the same number of dice will be rolled. ‘n’ is the input for no, and the user will be taken back to the start screen where they are asked how many dice they would like to roll. If a random key is pressed while being prompted to roll again, nothing happens. The entire time, a clock starting from 12:00:00 pm is keeping track of how long the game dice rolling has been being used for is displayed, along with the date for multi-day campaigns. This clock is updated every second using a timer to do so.
### Calculations
USARTDIV = BRR = 4,000,000/115200 = 35

## User’s Manual
The device starts upon being powered up. It begins at the start screen, where the user is prompted to roll between one to three six sided dice. This can be done by selecting ‘1’, ‘2’, or’3’ on your keyboard. If an invalid input is submitted, the device will remind the user to input a valid response.
The time and data is displayed at the top, in HH:MM:SS and year-month-date format.
Once a number is selected, the machine will then go and roll that many dice, with the resulting outputs being random. The dice face, dice number, and what all the dice sum to is then printed. The user is then asked if they would like to roll again. To select yes, press ‘y’ on your keyboard. This will go ahead and roll the same number of dice you selected earlier. To select no, press ‘n’ on your keyboard. This will take the user back to the start screen, where they can re-select how many dice they would like to roll. The dice rolling device is turned off upon being powered off.
