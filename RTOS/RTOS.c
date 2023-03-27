#include "RTOS_Interface.h"
#include "RTOS_Private.h"
#include "RTOS_Config.h"
#include "../MCAL/TIMER/timer.h"
#include "../MCAL/GIE/GIE_Interface.h"

static task_t rtos_tasks[MAX_TASKS] = {NULL};
// static uint8 rtos_tasks_first_open_slot = 0;

uint8 RTOS_create_task(f_ptr pToFunction, uint32 copy_periodicty, uint8 copy_priority, uint32 copy_firstDelay)
{
    uint8 rtos_status = OK;
    if (rtos_tasks[copy_priority].task_function == NULL) // first time to use this priority slot
    {
        rtos_tasks[copy_priority].task_function = pToFunction;
        rtos_tasks[copy_priority].periodicty = copy_periodicty;
        rtos_tasks[copy_priority].state = RESUME;
        rtos_tasks[copy_priority].firstDelay = copy_firstDelay;
    }
    else
        rtos_status = NOT_OK;
    return rtos_status;

    // rtos_tasks_first_open_slot++;
}

void RTOS_init()
{
    TIMER0_SetConfig();
    TIMER0_set_callback(RTOS_scheduler);
    sei();
    TIMER0_SysTick(SYSTICK_MS);
}

void RTOS_scheduler(void)
{
    for (int i = 0; i < MAX_TASKS; ++i)
    {
        if (rtos_tasks[i].task_function != NULL && rtos_tasks[i].state != SUSPEND)
        {
            if (rtos_tasks[i].firstDelay == 0)
            {
                (*(rtos_tasks[i].task_function))(); // call the task;
                rtos_tasks[i].firstDelay = rtos_tasks[i].periodicty - 1;
            }
            else
            {
                rtos_tasks[i].firstDelay--;
            }
        }
    }
}

void RTOS_resume_task(uint8 priority)
{

    rtos_tasks[priority].state = RESUME;
}
void RTOS_suspend_task(uint8 priority)
{
    rtos_tasks[priority].state = SUSPEND;
}
void RTOS_delete_task(uint8 priority)
{
    rtos_tasks[priority].task_function = NULL;
}