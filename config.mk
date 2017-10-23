################################################################################
# Project: odroid-xen-rtos
# 
# Module: project configuration
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

# Toolchain
TOOLCHAIN_ROOT = 
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
AR = $(PREFIX)ar
BIN = $(PREFIX)objcopy

LIB_C = 
LIB_GCC = 

# C source flags
CFLAGS = -Wall \
         -Werror \
         -mcpu=cortex-a15 \
         -mfpu=vfpv4 \
         -mthumb-interwork \
         -mfloat-abi=softfp \
         -fomit-frame-pointer \
         -fno-strict-aliasing \
         -nostdlib

INCLUDE = -I $(PROJECT_ROOT)/include/api \
          -I $(PROJECT_ROOT)/include/arch \
          -I $(PROJECT_ROOT)/include/drivers \
          -I $(PROJECT_ROOT)/include/lib \
          -I $(PROJECT_ROOT)/lib/FreeRTOS \
          -I $(PROJECT_ROOT)/lib/FreeRTOS/include \
          -I $(PROJECT_ROOT)/lib/FreeRTOS/portable/GCC/ARM_CA7/ \
          -I $(PROJECT_ROOT)/apps

CFLAGS += $(INCLUDE)

# Platform defines
CFLAGS += -D __XEN__ -O3

# Assembler flags
AFLAGS += -x assembler-with-cpp

# Linker flags
LDFLAGS = -static -nostartfiles -specs=nosys.specs

# ObjCopy flags
BINFLAGS = -O binary

# Linker script is architecture dependent
LINKER_SCRIPT = $(PROJECT_ROOT)/arch/arm/linker.lds
