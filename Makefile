mcu1_hal = ./HAL/LCD/LCD.c ./HAL/7SEG/7SEG.c
# mcu1_rtos = ./RTOS/RTOS.c
mcu1_rtos = ./FreeRTOS/tasks.c ./FreeRTOS/queue.c ./FreeRTOS/list.c ./FreeRTOS/port.c  ./FreeRTOS/heap_1.c  
# mcu1_mcal = ./MCAL/TIMER/timer.c ./MCAL/GIE/GIE.c ./MCAL/DIO/dio.c
mcu1_mcal =  ./MCAL/GIE/GIE.c ./MCAL/DIO/dio.c
# mcu1_lib = ./LIB/Queue/Queue.c
mcu1_app = 


SRC = main.c $(mcu1_app) $(mcu1_rtos) $(mcu1_hal) $(mcu1_mcal) $(mcu1_lib)




OBJ = $(subst .c,.o, $(SRC))

TARGET = freeRTOS


MCU = atmega32
FLASHER = usbasp
F_CPU = 16000000UL 
# F_CPU = 8000000UL 

HFUSE_BITS = 0xc2
HFUSE = hfuse:w:$(HFUSE_BITS):m
# AVRDUDE_ARGS = $(HFUSE)
AVRDUDE_ARGS =
CFLAGS = -Os  -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU)  
CFLAGS += -Wno-pointer-sign -Wno-strict-aliasing #disable pointer mismatch warning

LDFLAGS = -Wl,-Map,$(TARGET).map

INCLUDE  = -I./utils
INCLUDE += -I./MCAL
INCLUDE += -I./HAL
INCLUDE += -I./FreeRTOS/include
INCLUDE += -I/lib/avr/include

# avr lib
LD_AVR = -L/lib/avr/lib/avr5 -latmega32a
# LD_AVR =  
all: compile build flash

# default:
# 	@echo $(filter %led,$(SRC))

compile:$(TARGET)

$(TARGET):$(OBJ)
	@avr-gcc $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(LD_AVR) $(OBJ)  -o ./bin/$@.elf
	
%.o:%.c
	@echo "compiling $^"
	@avr-gcc $(CFLAGS) $(INCLUDE) $(LD_AVR) $^ -c  -o $@ 

build:
	@echo " building Hex file ... "
	@avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ./bin/$(TARGET).elf ./bin/$(TARGET).hex
	# @avr-objcopy  -j .text -j .data -O ihex ./bin/$(TARGET).elf ./bin/$(TARGET).hex

flash:build
	avrdude -c $(FLASHER) -p $(MCU)  -U flash:w:./bin/$(TARGET).hex -F
	# avrdude -c $(FLASHER) -p $(MCU)  -U flash:w:./bin/$(TARGET).hex 

clean:
	$(foreach file,$(OBJ),$(shell rm $(file)))


sim:
	simavr -g -m $(MCU) ./bin/main.elf
gdb:
	avr-gdb -x gdb-commands -tui 
