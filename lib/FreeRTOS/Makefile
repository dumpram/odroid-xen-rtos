################################################################################
# Project: odroid-xen-rtos
# 
# Module: FreeRTOS
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

include ../../config.mk

MODULE_NAME = FreeRTOS

SRC  = $(wildcard *.c)
SRC += $(wildcard portable/GCC/ARM_CA7/*.c)
SRC += $(wildcard portable/MemMang/heap_1.c)
	
ASRC = $(wildcard portable/GCC/ARM_CA7/*.S)

OBJ  = $(patsubst %.c, %.o, $(SRC))
AOBJ = $(patsubst %.S, %.o, $(ASRC))

all: $(OBJ) $(AOBJ)

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

%.o : %.S Makefile
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	@rm -f $(OBJ) $(AOBJ)
	@echo "$(MODULE_NAME): Clean complete"
