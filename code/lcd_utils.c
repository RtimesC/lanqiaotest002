#include "stm32g4xx.h" // Device header
#include "lcd.h"
#include <stdio.h>

extern int count;

void lcd_show()
{
	char text[20];
		sprintf(text, "        test        "); 
		LCD_DisplayStringLine(Line0, (uint8_t *)text);
		sprintf(text, "      count: %d     ", count); 
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
}
