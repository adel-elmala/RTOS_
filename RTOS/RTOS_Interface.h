#ifndef RTOS_INTERFACE_H
#define RTOS_INTERFACE_H
#include "RTOS_Private.h"

#define TASK_PRIORITY_0 0
#define TASK_PRIORITY_1 1
#define TASK_PRIORITY_2 2
#define TASK_PRIORITY_3 3
#define TASK_PRIORITY_4 4

void RTOS_init();
void RTOS_scheduler(void);
uint8 RTOS_create_task(f_ptr pToFunction, uint32 copy_periodicty, uint8 copy_priority, uint32 copy_firstDelay);

void RTOS_resume_task(uint8 priority);
void RTOS_suspend_task(uint8 priority);
void RTOS_delete_task(uint8 priority);

#endif