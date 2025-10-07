#include <xc.h>
#include "lcd.h"
#include "adc.h"

#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF

void displayVoltage(unsigned int adcVal);

void main(void) {
    unsigned int adcVal;

    // Initialize ports and LCD
    TRISB = 0x00;
    PORTB = 0x00;
    TRISA = 0b00000100; // RA2 as input (ADC)
    PORTA = 0x00;

    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Voltage:");

    while(1) {
        adcVal = readADC(); // 10-bit ADC value (0?1023)
        displayVoltage(adcVal);
        __delay_ms(500);
    }
}

// Display voltage in format X.XXX V using integer math
void displayVoltage(unsigned int adcVal) {
    unsigned int millivolts = (adcVal * 5000L) / 1023;
    unsigned char volts = millivolts / 1000;
    unsigned int frac = millivolts % 1000;

    char buffer[17]; // 16 chars + null terminator

    // Manually build the string: "X.XXX V"
    buffer[0] = volts + '0';
    buffer[1] = '.';
    buffer[2] = (frac / 100) % 10 + '0';
    buffer[3] = (frac / 10) % 10 + '0';
    buffer[4] = frac % 10 + '0';
    buffer[5] = ' ';
    buffer[6] = 'V';
    buffer[7] = '\0';

    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String("        ");   // Clear line
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String(buffer);      // Write voltage
}
