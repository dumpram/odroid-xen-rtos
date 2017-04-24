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

SUBMODULES = api arch drivers lib apps
APPS = APPS

all: export

obj/app.elf : submodules
	$(eval OBJECTS = $(shell find $(SUBMODULES) -name *.o))
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
	$(foreach submodule,$(SUBMODULES),$(MAKE) -C $(submodule) && ) true
	+$(MAKE) -C apps

clean:
	+$(MAKE) -C api clean
	+$(MAKE) -C arch clean
	+$(MAKE) -C drivers clean
	+$(MAKE) -C lib clean
	+$(MAKE) -C apps clean
	rm obj/app.elf
	rm app.bin

export: app.bin
	scp app.bin odroid:~