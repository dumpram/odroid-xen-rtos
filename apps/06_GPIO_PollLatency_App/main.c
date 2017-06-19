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

#include <stdio.h>
#include <stdbool.h>

// pin numbers correspond to ODROID-XU3 header
#define GPIO_PIN_HEARTBEAT 24
#define GPIO_PIN_OUT       10
#define GPIO_PIN_IN        26

bool state = true;

xSemaphoreHandle xBinarySemaphore;

void init_test()
{
    gpio_init(GPIO_PIN_IN, GPIO_MODE_IN);
    gpio_init(GPIO_PIN_OUT, GPIO_MODE_OUT);
}

void vTaskHeartBeat()
{
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

void vTaskPoll()
{
    int value, new_value;
    init_test();

    value = gpio_get_value(GPIO_PIN_IN);
    while(1)
    {
        new_value = gpio_get_value(GPIO_PIN_IN);
        if (value != new_value)
        {
            value = new_value;
            gpio_set_value(GPIO_PIN_OUT, value);
        }
    }
}


int main()
{
    print_simple("Entered main!\n");

    int ret = xTaskCreate(vTaskPoll, "Task 1", 500, NULL, 1, NULL);
    if (ret == pdPASS) 
    {
        print_simple("Poll task succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }

    vTaskStartScheduler();

    while (1);
}