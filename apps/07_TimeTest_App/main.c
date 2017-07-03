/**
 * @ brief:
 *
 * FreeRTOS Time test application.
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
#include <time.h>
#include <clocksource/gt.h>

#include <stdio.h>
#include <stdbool.h>

// pin numbers correspond to ODROID-XU3 header
#define GPIO_PIN_HEARTBEAT 24
#define GPIO_PIN_OUT       10
#define GPIO_PIN_IN        26


void vTaskTimeMeasureReferent()
{
    uint64_t time;

    print_simple("Task!\r\n");

    while(1)
    {
        time = gettimeofday();
        vTaskDelay(configTICK_RATE_HZ);
        time = gettimeofday() - time;
        print_register("time", time);
    }
}

void vTaskTimeMeasure()
{
    uint64_t start;
    uint32_t i = 0;

    print_simple("Task!\r\n");

    start = gt_get_cntvct();

    while (i < 1000000)
    {
        i++;
    }

    print_register("time", gt_get_cntvct() - start);

    while (1)
    {

    }
}

int main()
{
    int ret = xTaskCreate(vTaskTimeMeasure, "Task 1", 500, NULL, 1, NULL);

    if (ret == pdPASS) 
    {
        print_simple("Time measure task succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }

//    vTaskStartScheduler();
    vTaskTimeMeasure();

    while (1);
}