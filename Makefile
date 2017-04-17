###############################################################################
# Makefile for OdroidXU3-Xen-FreeRTOS project
# v 1.0
# Author: Ivan Pavic
###############################################################################

# Toolchain
TOOLCHAIN_ROOT = /home/dumpram/arm-compilers/gcc-arm-none-eabi-5_4-2016q3

#PREFIX = ~/arm-compilers/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-
PREFIX = arm-linux-gnueabihf-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
AR = $(PREFIX)ar
BIN = $(PREFIX)objcopy

APP_LOAD_ADDR = 0x80000000 # LOAD IN RAM

LIB_C = $(TOOLCHAIN_ROOT)/arm-none-eabi/lib
LIB_GCC = $(TOOLCHAIN_ROOT)/lib/gcc/arm-none-eabi/5.4.1

# User source files
USER_SRC = $(wildcard src/*.c)

# User assembler files
USER_ASRC = $(wildcard src/asm/*.s)

# RTOS root
FREERTOS_ROOT = lib/FreeRTOS

# RTOS platform dependent
FREERTOS_PORT = $(FREERTOS_ROOT)/portable/GCC/ARM_CA7

# RTOS portable common
FREERTOS_PORT_COMMON = $(FREERTOS_ROOT)/portable/Common

# RTOS memory management
FREERTOS_MEMMANG = $(FREERTOS_ROOT)/portable/MemMang

# Gathering source files
RTOS_SRC := $(wildcard $(FREERTOS_ROOT)/*.c)
RTOS_SRC += $(wildcard $(FREERTOS_PORT)/*.c)
RTOS_SRC += $(wildcard $(FREERTOS_MEMMANG)/heap_1.c)

RTOS_INC := $(FREERTOS_ROOT)/include

RTOS_ASRC += $(wildcard $(FREERTOS_PORT)/*.S)

# RTOS Objects
RTOS_OBJ := $(patsubst %.c, %.o, $(RTOS_SRC))
RTOS_AOBJ := $(patsubst %.S, %.o, $(RTOS_ASRC))

LINKER_SCRIPT = linker.lds

BINFLAGS = -O binary

# User Objects
USER_OBJ := $(patsubst %.c, %.o, $(USER_SRC))
USER_AOBJ := $(patsubst %.s, %.o, $(USER_ASRC))

# Includes
CFLAGS += -I$(RTOS_INC) -I . -I $(FREERTOS_PORT) -I inc $(PLATFORM_INC)

# Machine flags
CFLAGS +=-Wall \
		-mcpu=cortex-a7 \
        -mfpu=vfpv4 \
        -fomit-frame-pointer \
		-fno-strict-aliasing \
		-nostdlib

AFLAGS += -x assembler-with-cpp

# Linker flags
LDFLAGS :=-static -nostartfiles -Xlinker -build-id=none -T$(LINKER_SCRIPT)

all : app.bin

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

%.o: %.s Makefile
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

%.o: %.S Makefile
	@$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

obj/app.elf : $(RTOS_OBJ) $(RTOS_AOBJ) $(USER_AOBJ) $(USER_OBJ) 
	@rm -rf obj/app.elf
	@$(LD) $(LDFLAGS) -o $@ $(USER_AOBJ) $(USER_OBJ) $(RTOS_OBJ) $(RTOS_AOBJ)
	@echo "Linked app successfully!"

%.bin : obj/%.elf
	@rm -rf app.bin
	@$(BIN) $< $(BINFLAGS) $@
	@echo "Generated binary successfully!"

clean :
	rm -f $(RTOS_OBJ) $(RTOS_AOBJ) $(USER_OBJ) $(USER_AOBJ) \
	obj/app.elf app.bin

dirs :
	@mkdir -p obj && echo "Created object directory..."

export: app.bin
	scp app.bin root@192.168.0.2:~
	@echo "App exported successfully!"
