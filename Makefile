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

all: app.bin

obj/app.elf : submodules
	$(eval OBJECTS = $(shell find . -name *.o))
	$(LD) $(OBJECTS) \
	$(LDFLAGS) -o $@ \
	-L$(LIB_GCC) -L$(LIB_C) \
	-lgcc -lc  \
	-T$(LINKER_SCRIPT)
	@echo "Linked app successfully!"

%.bin : obj/%.elf
	@$(BIN) $< $(BINFLAGS) $@
	@echo "Generated binary successfully!"

submodules:
	+$(MAKE) -C api
	+$(MAKE) -C arch
	+$(MAKE) -C drivers
	+$(MAKE) -C lib


clean:
	+$(MAKE) -C api clean
	+$(MAKE) -C arch clean
	+$(MAKE) -C drivers clean
	+$(MAKE) -C lib clean
	rm obj/app.elf
	rm app.bin
