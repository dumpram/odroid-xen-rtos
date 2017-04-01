###############################################################################
# Makefile for OdroidXU3-Xen-FreeRTOS project
# v 1.0
# Author: Ivan Pavic
###############################################################################

# Toolchain
TOOLCHAIN_ROOT = /home/dumpram/arm-compilers/gcc-arm-none-eabi-5_4-2016q3

PREFIX = ~/arm-compilers/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)ld
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


# Compiler flags
CFLAGS := -I$(RTOS_INC) -I . -I $(FREERTOS_PORT) -I inc $(PLATFORM_INC) \
	-mcpu=cortex-a7 \
	-mfpu=vfpv4 \
	-mthumb-interwork \
	-mfloat-abi=softfp

# Linker flags
LDFLAGS := 

all : app.bin dirs

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

%.o: %.S
	@$(AS) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

obj/app.elf : dirs $(RTOS_OBJ) $(RTOS_AOBJ) $(USER_OBJ)
	@$(LD) $(LDFLAGS) -o $@ $(USER_OBJ) $(RTOS_OBJ) $(RTOS_AOBJ) \
		-L$(LIB_C) -L$(LIB_GCC) -lc -lgcc -T$(LINKER_SCRIPT)
	@echo "Linked app successfully!"

app.bin : obj/app.elf
	@$(BIN) $(BINFLAGS) obj/app.elf app.bin
	@echo "Generated binary successfully!"

clean :
	rm -f $(RTOS_OBJ) $(RTOS_AOBJ) $(USER_OBJ) \
	obj/app.elf app.bin

dirs :
	@mkdir -p obj && echo "Created object directory..."

export: app.bin
	scp app.bin root@192.168.0.2:~
	@echo "App exported successfully!"
