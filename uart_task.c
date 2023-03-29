#include "./MCAL/DIO/DIO_Interface.h"
#include "./MCAL/DIO/DIO_REG.h"
#include "./MCAL/UART/UART_Interface.h"
#include "./HAL/LCD/LCD_Interface.h"

#include "./FreeRTOS/include/FreeRTOS.h"
#include "./FreeRTOS/include/task.h"
#include "./FreeRTOS/include/semphr.h"
#include "LIB/Queue/Queue.h"

void lcd_task(void *);
void uart_task(void *input);

SemaphoreHandle_t uart_sem;
Queue_Circular_t uart_recv_q = {0};

UART_Config_t uart_conf =
    {
        9600,                                           // uint16 baud_rate;
        UART_DISABLE_DOUBLE_TRANSMISSION_SPEED,         // uint8 enable_double_transmission_speed;
        UART_DISABLE_MULTIPROCESSOR_COMMUNICATION_MODE, // uint8 enable_multiprocessor_mode;
        UART_DISABLE_RECEIVE_COMPLETE_INTERRUPT,        // uint8 enable_recevie_complete_interrupt;
        UART_DISABLE_TRANSMIT_COMPLETE_INTERRUPT,       // uint8 enable_transmit_complete_interrupt;
        UART_DISABLE_DATA_REG_EMPTY_COMPLETE_INTERRUPT, // uint8 enable_data_reg_empty_complete_interrupt;
        UART_ENABLE_RECEIVER,                           // uint8 enable_receiver;
        UART_ENABLE_TRANSMITER,                         // uint8 enable_transmiter;
        UART_8BIT_CHAR_SIZE,                            // uint8 frame_data_bits_size;
        UART_DISABLE_PARITY,                            // uint8 parity_mode;
        UART_1_BIT_STOP                                 // uint8 stop_bits_mode;
};
int main(int argc, char const *argv[])
{

    uart_sem = xSemaphoreCreateCounting(16, 0);

    lcd_init();
    UART_vInit(uart_conf);

    if (xTaskCreate(lcd_task, "lcd_task", 128, NULL, 1, NULL) != pdPASS)
        lcd_displayString("Failed  t1");
    if (xTaskCreate(uart_task, "uart_task", 128, NULL, 2, NULL) != pdPASS)
        lcd_displayString("Failed  t2");

    vTaskStartScheduler();
    while (1)
    {
    }

    return 0;
}

void lcd_task(void *in)
{
    uint8 counter = 0;
    while (1)
    {
        if (xSemaphoreTake(uart_sem, 5) == pdTRUE)
        {
            uint8 data = q_dequeue(&uart_recv_q);
            lcd_sendData(data);
            counter++;
            if (counter == 16)
            {
                lcd_goto_line2();
            }
            if (counter > 32)
            {
                lcd_goto(LCD_LINE_1, LCD_COL_1);
                counter = 0;
            }
        }
        vTaskDelay(100);
    }
}

void uart_task(void *input)
{
    uint16 data;
    while (1)
    {
        if ((UCSRA & (1 << UCSRA_RXC)))
        {
            data = UDR;
            q_enqueue(&uart_recv_q, data);
            xSemaphoreGive(uart_sem);
        }
        vTaskDelay(5);
    }
}
