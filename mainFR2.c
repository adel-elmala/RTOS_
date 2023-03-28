#include "MCAL/DIO/DIO_Interface.h"
#include "MCAL/DIO/DIO_REG.h"
#include "MCAL/GIE/GIE_Interface.h"
// #include "MCAL/TIMER/timer.h"
#include "HAL/LCD/LCD_Interface.h"
#include "HAL/7SEG/7SEG_Interface.h"
#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"

void led_task(void *);
void lcd_task(void *);

void refresh_ssd1(void *in);
void refresh_ssd2(void *in);
void sevenSeg_count(void *in);

static uint8 seg1_counter = 0;
static uint8 seg2_counter = 0;
int main(int argc, char const *argv[])
{
    DIO_vSetPinDirection(__PORTA__, PIN5, OUTPUT);
    DIO_vSetPinDirection(__PORTA__, PIN0, OUTPUT);
    DIO_vSetPinDirection(__PORTA__, PIN1, OUTPUT);
    lcd_init();
    sevenSeg1_init();
    sevenSeg2_init();

    if (xTaskCreate(led_task, "led_task", 128, NULL, 0, NULL) != pdPASS)
        lcd_displayString("Failed  t1");

    if (xTaskCreate(sevenSeg_count, "ssd1_task", 128, NULL, 4, NULL) != pdPASS)
        lcd_displayString("Failed  t2");

    if (xTaskCreate(refresh_ssd1, "ssd2_task", 128, NULL, 3, NULL) != pdPASS)
        lcd_displayString("Failed  t3");

    if (xTaskCreate(lcd_task, "lcd_task", 128, NULL, 1, NULL) != pdPASS)
        lcd_displayString("Failed  t4");

    if (xTaskCreate(refresh_ssd2, "refresh_ssd_task", 128, NULL, 2, NULL) != pdPASS)
        lcd_displayString("Failed  t5");

    vTaskStartScheduler();
    while (1)
    {
    }

    return 0;
}

void led_task(void *in)
{
    while (1)
    {
        DIO_vTogglePin(__PORTA__, PIN5);

        vTaskDelay(1000);
    }
}

void lcd_task(void *in)
{
    uint8 str[] = "Hello NTI!";
    static uint8 counter = 0;
    while (1)
    {
        if (counter < 11)
            lcd_sendData(str[counter++]);
        vTaskDelay(1000);
    }
}
void sevenSeg_count(void *in)
{
    while (1)
    {
        // DIO_vTogglePin(__PORTA__, PIN6);
        seg1_counter++;
        if (seg1_counter > 9)
        {
            seg1_counter = 0;
            seg2_counter++;
        }
        vTaskDelay(1000);
    }
}

void refresh_ssd1(void *in)
{
    while (1)
    {
        DIO_vWritePin(__PORTA__, PIN0, LOW);
        DIO_vWritePin(__PORTA__, PIN1, HIGH);
        sevenSeg1_display_digit(seg1_counter);
        vTaskDelay(10);
    }
}

void refresh_ssd2(void *in)
{
    while (1)
    {

        DIO_vWritePin(__PORTA__, PIN0, HIGH);
        DIO_vWritePin(__PORTA__, PIN1, LOW);
        sevenSeg2_display_digit(seg2_counter);
        vTaskDelay(20);
    }
}