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


