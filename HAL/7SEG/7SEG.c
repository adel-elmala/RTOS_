#include "7SEG_Interface.h"
#include "7SEG_Private.h"
#include "../../MCAL/DIO/DIO_Interface.h"
void sevenSeg1_init()
{
    DIO_vSetPortDirection(SEVSEG_PORT, OUTPUT);
}
void sevenSeg1_display_digit(uint8 digit)
{
    char array[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    DIO_vWritePort(SEVSEG_PORT, array[digit]);
}

void sevenSeg2_init()
{
    DIO_vSetPortDirection(SEVSEG2_PORT, OUTPUT);
}
void sevenSeg2_display_digit(uint8 digit)
{
    char array[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    DIO_vWritePort(SEVSEG2_PORT, array[digit]);
}
