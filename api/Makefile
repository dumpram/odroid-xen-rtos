################################################################################
# Project: odroid-xen-rtos
# 
# Module: api
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

include ../config.mk

MODULE_NAME = api

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(OBJ)

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

clean:
	@rm -f $(OBJ)
	@echo "$(MODULE_NAME): Clean complete"
