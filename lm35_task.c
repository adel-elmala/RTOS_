#include "MCAL/DIO/DIO_Interface.h"
#include "MCAL/DIO/DIO_REG.h"
#include "MCAL/GIE/GIE_Interface.h"
#include "MCAL/ADC/ADC_Interface.h"
#include "HAL/LCD/LCD_Interface.h"

#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"
#include "FreeRTOS/include/semphr.h"

#include <stdio.h>
void lcd_task(void *);
void adc_callBack(uint16 adc_result);

SemaphoreHandle_t adc_sem;
uint16 g_result = 0;
int main(int argc, char const *argv[])
{
    ADC_Config_t adc_conf = {ADC_REFVOLTAGE_INTERNAL,
                             ADC_LEFT_ADJUST,
                             ADC_SINGLE_ENDDED_ADC1,
                             ADC_ENABLE_INTR,
                             ADC_DISABLE_AUTOTRIGGER,
                             ADC_AUTO_TRIGGER_SOURCE_IGNORE};

    adc_sem = xSemaphoreCreateCounting(1, 0);

    lcd_init();
    ADC_init(adc_conf);
    ADC_setCallBack(adc_callBack);
    start_conversion();

    if (xTaskCreate(lcd_task, "lcd_task", 128, NULL, 1, NULL) != pdPASS)
        lcd_displayString("Failed  t1");

    vTaskStartScheduler();
    while (1)
    {
    }

    return 0;
}

void lcd_task(void *in)
{
    uint8 str[16] = {0};
    while (1)
    {
        if (xSemaphoreTake(adc_sem, 5) == pdTRUE)
        {
            sprintf(str, "temp: %d", g_result);
            lcd_clearAndHome();
            lcd_displayString(str);
        }
        vTaskDelay(100);
    }
}

void adc_callBack(uint16 adc_result)
{
    g_result = adc_result / 4;
    xSemaphoreGive(adc_sem);
    // xSemaphoreGiveFromISR(adc_sem, NULL);
}