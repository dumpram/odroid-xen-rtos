# odroid-xen-rtos
FreeRTOS port on Xen and Odroid XU3 platform (Exynos5422 SoC)

## How to port FreeRTOS to new platform?

When one is porting FreeRTOS to new platform you should be familiar with the interrupt controller and timers of platform. One should provide implementation of:

> void vSetupTickInterrupt()

Additionally one should handle interrupt by calling FreeRTOS kernel scheduler which will make rescheduling if necessary. Example of implementation of tick handler is: 

```
void vTickInterrupt()
{
  DISABLE_TICK_INTERRUPT();
  if (xTaskIncrementTick() != pdFALSE)
  {
    ulPortYieldRequired = pdTRUE;    
  }
  ENABLE_TICK_INTERRUPT();
}
```
FreeRTOS doesn't do context switching. Context switching depends on architecture and it is done in assembly. More details at:
http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html

FreeRTOS 9.0.0. comes with port for Cortex-A9 which is more or less compatible with Cortex-A7. Port for FreeRTOS only needs base address of GIC CPU interface. In this specific case this should be ammended little bit because MMU is used so address of GIC isn't fixed as it is expected in portable part of FreeRTOS code. Through FreeRTOSConfig.h one can provide macros for GIC manipulation. 

## How to install Xen on Odroid XU3(4)

On this link you can find very good instructions for installing Xen with Linux as domain 0 on Odroid XU3:

http://odroid.com/dokuwiki/doku.php?id=en:xu4_xen

## Further work

In this project, goal is to implement and test:

1) Xen drivers and pv protocols as they are implemented in Mini-OS
2) Drivers for exynos5422 gpio, spi and i2s peripherals
3) Test real-time performances using Linux and FreeRTOS in combination


