#ifndef RTOS_PRIVATE_H
#define RTOS_PRIVATE_H

#include "../LIB/Datatypes.h"

#define NOT_OK 0
#define OK 1

#define RESUME 0
#define SUSPEND 1

typedef void (*f_ptr)(void);
typedef struct
{
    f_ptr task_function;
    uint32 periodicty;
    uint8 state;
    uint8 firstDelay;
    // uint8 priority;
} task_t;

#endif