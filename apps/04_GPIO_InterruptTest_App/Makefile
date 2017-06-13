################################################################################
# Project: odroid-xen-rtos
# 
# Module: apps/04_GPIO_InterruptTest_App
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

include ../../config.mk

MODULE_NAME = $(notdir $(shell pwd))

SRC  = $(wildcard *.c)
OBJ  = $(patsubst %.c, %.o, $(SRC))

ASRC = $(wildcard *.s)
AOBJ = $(patsubst %.s, %.o, $(ASRC))

all: $(MODULE_NAME).bin

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

%.o : %.s Makefile
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled "$<" successfully!"

%.elf : $(AOBJ) $(OBJ)
	@$(LD) $(MODULES) $(AOBJ) $(OBJ) \
	$(LDFLAGS) -o $@ \
	-L$(LIB_GCC) -L$(LIB_C) \
	-lgcc -lc  \
	-T$(LINKER_SCRIPT)
	@echo "$(MODULE_NAME): Linked app successfully!"

$(MODULE_NAME).bin : $(MODULE_NAME).elf
	@$(BIN) $< $(BINFLAGS) $@
	@echo "$(MODULE_NAME): Generated binary successfully!"

clean:
	@rm -f $(AOBJ) $(OBJ)
	@echo "$(MODULE_NAME): Clean complete"
	@rm -f $(MODULE_NAME).bin
	@rm -f $(MODULE_NAME).elf