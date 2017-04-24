################################################################################
# Project: odroid-xen-rtos
# 
# Module: apps
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

APPS = $(shell find . -type d -name "*" -not -path ".")


all:
	$(foreach app,$(APPS),$(MAKE) -C $(app) && ) true

clean:
	$(foreach app,$(APPS),$(MAKE) -C $(app) clean && ) true