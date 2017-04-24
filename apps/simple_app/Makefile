################################################################################
# Project: odroid-xen-rtos
# 
# Module: apps/simple_app
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

include ../../config.mk

MODULE_NAME = simple_app

SRC  = $(wildcard *.c)
OBJ  = $(patsubst %.c, %.o, $(SRC))

ASRC = $(wildcard *.s)
AOBJ = $(patsubst %.s, %.o, $(ASRC))

all: $(OBJ) $(AOBJ)

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

%.o : %.s Makefile
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	@rm -f $(AOBJ) $(OBJ)
	@echo "$(MODULE_NAME): Clean complete"
