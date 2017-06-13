################################################################################
# Project: odroid-xen-rtos
# 
# Module: drivers
# 
# Author: Ivan Pavic (@dumpram)
#
# version: 0.0.0
################################################################################

all:
	+$(MAKE) -C clocksource
	+$(MAKE) -C irqchip
	+$(MAKE) -C gpio
	+$(MAKE) -C exti

clean:
	+$(MAKE) -C clocksource clean
	+$(MAKE) -C irqchip clean
	+$(MAKE) -C gpio clean
	+$(MAKE) -C exti clean