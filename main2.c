#include "MCAL/DIO/DIO_Interface.h"
#include "MCAL/DIO/DIO_REG.h"
#include "MCAL/GIE/GIE_Interface.h"
// #include "MCAL/TIMER/timer.h"
#include "HAL/LCD/LCD_Interface.h"
#include "HAL/7SEG/7SEG_Interface.h"
#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/semphr.h"

void lcd_task(void *);
void lcd_task2(void *);
// uint8 flag = 1;
static SemaphoreHandle_t lcd_sem_handle;
static SemaphoreHandle_t lcd_sem_count_handle;

int main(int argc, char const *argv[])
{
    DIO_vSetPinDirection(__PORTA__, PIN5, OUTPUT);
    lcd_init();
    sevenSeg1_init();
    sevenSeg2_init();
    lcd_sem_handle = xSemaphoreCreateBinary();
    lcd_sem_count_handle = xSemaphoreCreateCounting(1, 1);

    if (xTaskCreate(lcd_task, "led_task", 128, NULL, 2, NULL) != pdPASS)
        lcd_displayString("Failed  t1");

    if (xTaskCreate(lcd_task2, "lcd_task2", 128, NULL, 2, NULL) != pdPASS)
        lcd_displayString("Failed  t2");

    vTaskStartScheduler();
    while (1)
    {
    }

    return 0;
}

void lcd_task(void *in)
{
    while (1)
    {
        xSemaphoreTake(lcd_sem_handle, 0);

        lcd_displayString("cccccccccccccccccc 1");
        xSemaphoreGiveFromISR(lcd_sem_handle, NULL);
        vTaskDelay(1);
    }
}

void lcd_task2(void *in)
{
    while (1)
    {
        xSemaphoreTake(lcd_sem_handle, 0);

        lcd_displayString("KKKKKKKKKKKKKKKKKKK 2");
        xSemaphoreGiveFromISR(lcd_sem_handle, NULL);

        vTaskDelay(1);
    }
}
