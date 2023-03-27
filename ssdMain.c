#include "MCAL/DIO/DIO_Interface.h"
#include "MCAL/DIO/DIO_REG.h"
#include "MCAL/GIE/GIE_Interface.h"
#include "MCAL/TIMER/timer.h"
#include "HAL/LCD/LCD_Interface.h"
#include "HAL/7SEG/7SEG_Interface.h"
#include "RTOS/RTOS_Interface.h"

void toggle_led_1(void);
void sevenSeg1_task();
void sevenSeg2_task();
void lcd_task();

int main(int argc, char const *argv[])
{
    lcd_init();
    sevenSeg1_init();
    sevenSeg2_init();
    DIO_vSetPinDirection(PORTA, PIN7, OUTPUT); // led pin direction

    RTOS_create_task(toggle_led_1, 1, TASK_PRIORITY_0, 0);
    RTOS_create_task(lcd_task, 5, TASK_PRIORITY_1, 0);
    RTOS_create_task(sevenSeg1_task, 1, TASK_PRIORITY_2, 0);
    RTOS_create_task(sevenSeg2_task, 10, TASK_PRIORITY_3, 0);
    RTOS_init();
    while (1)
    {
    }

    return 0;
}

void toggle_led_1(void)
{
    DIO_vTogglePin(PORTA, PIN7);
}

void lcd_task()
{
    uint8 str[] = "Hello NTI!";
    static uint8 counter = 0;
    if (counter < 12)
        lcd_sendData(str[counter++]);
}
void sevenSeg1_task()
{

    static uint8 seg1_counter = 0;

    sevenSeg1_display_digit(seg1_counter++);
    if (seg1_counter > 9)
        seg1_counter = 0;
}

void sevenSeg2_task()
{

    static uint8 seg2_counter = 0;

    sevenSeg2_display_digit(seg2_counter++);
    if (seg2_counter > 9)
        seg2_counter = 0;
}
