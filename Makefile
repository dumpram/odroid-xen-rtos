################################################################################
# Project: odroid-xen-rtos
# 
# Module: top module
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################
include config.mk

PROJECT_ROOT = $(shell pwd)
export PROJECT_ROOT

SUBMODULES = $(PROJECT_ROOT)/api $(PROJECT_ROOT)/arch $(PROJECT_ROOT)/drivers \
			 $(PROJECT_ROOT)/lib

all: export

submodules:
	$(foreach submodule,$(SUBMODULES),$(MAKE) -C $(submodule) && ) true
    
apps: submodules
	$(eval OBJECTS = $(shell find $(SUBMODULES) -name *.o))
	+$(MAKE) -C apps MODULES="$(OBJECTS)"

clean:
	$(foreach submodule,$(SUBMODULES),$(MAKE) -C $(submodule) clean && ) true
	+$(MAKE) -C apps clean

export: apps
	scp apps/04_GPIO_InterruptTest_App/04_GPIO_InterruptTest_App.bin odroid:~/app.bin
