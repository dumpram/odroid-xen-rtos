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

// pin numbers correspond to ODROID-XU3 header
#define GPIO_PIN_OUT 10
#define GPIO_PIN_IN  24


xSemaphoreHandle xBinarySemaphore;

void vTaskHeartBeat()
{
    gpio_init(GPIO_PIN_OUT, 1);
    gpio_set_value(GPIO_PIN_OUT, 0);

    while(1)
    {
        gpio_set_value(GPIO_PIN_OUT, 1);
        vTaskDelay(configTICK_RATE_HZ / 4);
        gpio_set_value(GPIO_PIN_OUT, 0);
        vTaskDelay(configTICK_RATE_HZ / 4);
        gpio_set_value(GPIO_PIN_OUT, 1);
        vTaskDelay(configTICK_RATE_HZ / 4);
        gpio_set_value(GPIO_PIN_OUT, 0);
        vTaskDelay(configTICK_RATE_HZ / 2);
    }
}

void external_irq_handler()
{
    print_simple("Interrupt occured! :D\n");
}

void vTaskPinMonitor()
{
    int value, err;
    gpio_init(GPIO_PIN_IN, GPIO_MODE_EXTINT);
    
    value = gpio_get_value(GPIO_PIN_IN);

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

    print_simple("Prije petlje\n");

    while(1)
    {
        value = 1;
        if (value)
        {

        }
        // while (value == gpio_get_value(GPIO_PIN_IN))
        // {
        //     //vTaskDelay(configTICK_RATE_HZ); // wait 1 second
        //     //print_register("pin_value", value);
        // }
        // print_simple("State changed!\n");
        // value = gpio_get_value(GPIO_PIN_IN);
    }
}

int main()
{
    print_simple("Entered main!\n");

    xBinarySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xBinarySemaphore);

    int ret = xTaskCreate(vTaskHeartBeat, "Task 1", 500, NULL, 1, NULL);
    if (ret == pdPASS) 
    {
        print_simple("Task 1 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }
    
    ret =  xTaskCreate(vTaskPinMonitor, "Task 2", 500, NULL, 2, NULL);
    if (ret == pdPASS)
    {
        print_simple("Task 2 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }

    //vTaskStartScheduler();

    vTaskPinMonitor();

    while (1);
}