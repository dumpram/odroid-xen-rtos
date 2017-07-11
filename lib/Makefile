################################################################################
# Project: odroid-xen-rtos
# 
# Module: lib
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

all:
	+$(MAKE) -C FreeRTOS
	+$(MAKE) -C utils
	+$(MAKE) -C xen

clean:
	+$(MAKE) -C FreeRTOS clean
	+$(MAKE) -C utils clean
	+$(MAKE) -C xen clean