/**
 * @ brief:
 *
 * FreeRTOS and GPIO test application.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <utils/print.h>
#include <gpio.h>
#include <interrupt.h>
#include <exti.h>
#include <clocksource/gt.h>
#include <stdio.h>
#include <stdbool.h
#include <memory.h>
// pin numbers correspond to ODROID-XU3 header
#define GPIO_PIN_HEARTBEAT 24
#define GPIO_PIN_OUT       10
#define GPIO_PIN_IN        26
bool state = true;
xSemaphoreHandle xBinarySemaphore;
static void external_irq_handler()
{
    exti_clear_pend(0);
    gpio_set_value(GPIO_PIN_OUT, gpio_get_value(GPIO_PIN_IN));
}   
void init_test()
{
    int err;
    gpio_init(GPIO_PIN_IN, GPIO_MODE_EXTINT);
    gpio_init(GPIO_PIN_OUT, GPIO_MODE_OUT);
    gpio_set_value(GPIO_PIN_OUT, 1);
    memory_set_device(0x14010000);
    for (int i = 0; i < 8; i++)
    {
        // set trigger on all lines
        exti_set_trigger(i, EXTI_MODE_TRIG_BOTH); 
        // clear interrupt if pending
        exti_clear_pend(i); 
    }
    // mask all exti interrupts
    for (int i = 0; i < 8; i++)
    {
        exti_mask_irq(i, 0x1); 
    }
    // unmask eint16
    exti_mask_irq(0, 0x0); 
    err = interrupt_register_handler(64, external_irq_handler);
    if (!err)
    {
        print_simple("Interrupt handler registered succesfully!\n");
    }
    err = interrupt_set_priority(64, 0);
    if (!err)
    {
        print_simple("Interrupt priority set succesfully!\n");
    } 
    else
    {
        print_simple("Interrupt priority set unsuccessful!\n");
    }
    err = interrupt_enable_irq(64, 0);
    if (!err)
    {
        print_simple("Interrupt enabled succesfully!\n");
    }
}
void vTaskHeartBeat()
{
    init_test();
    gpio_init(GPIO_PIN_HEARTBEAT, 1);
    gpio_set_value(GPIO_PIN_HEARTBEAT, 0);
    while(1)
    {
        gpio_set_value(GPIO_PIN_HEARTBEAT, 1);
        vTaskDelay(configTICK_RATE_HZ / 8);
        gpio_set_value(GPIO_PIN_HEARTBEAT, 0);
        vTaskDelay(configTICK_RATE_HZ / 8);
        gpio_set_value(GPIO_PIN_HEARTBEAT, 1);
        vTaskDelay(configTICK_RATE_HZ / 8);
        gpio_set_value(GPIO_PIN_HEARTBEAT, 0);
        vTaskDelay(configTICK_RATE_HZ / 2);
    }
}
int main()
{
    print_simple("Entered main!\n");
    int ret = xTaskCreate(vTaskHeartBeat, "Task 1", 500, NULL, 1, NULL);
    if (ret == pdPASS) 
    {
        print_simple("Task 1 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }
    init_test();
    while(1)
    {
    }
}