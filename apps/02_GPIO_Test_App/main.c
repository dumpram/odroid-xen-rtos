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


xSemaphoreHandle xBinarySemaphore;

void vTask1()
{
    int i = 0;

    while(1)
    {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        print_register("Task 1", i++);
        xSemaphoreGive(xBinarySemaphore);
        vTaskDelay(configTICK_RATE_HZ / 2);
    }
}

void vTask2()
{
    int i = 0;

    while(1)
    {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        print_register("Task 2", i++);
        xSemaphoreGive(xBinarySemaphore);
        vTaskDelay(configTICK_RATE_HZ * 1);
    }
}

void vTask3()
{
    TickType_t lastWakeUp;
    gpio_init(2 * 8 + 4, 1);
    gpio_set_value(2 * 8 + 4, 0);

    lastWakeUp = xTaskGetTickCount();
    while(1)
    {
        vTaskDelayUntil(&lastWakeUp, 10);
        gpio_set_value(2 * 8 + 4, 1);
        vTaskDelayUntil(&lastWakeUp, 10);
        gpio_set_value(2 * 8 + 4, 0);
    }
}

int main()
{
    print_simple("Entered main!\n");

    xBinarySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xBinarySemaphore);

    int ret = xTaskCreate(vTask1, "Task 1", 500, NULL, 1, NULL);
    if (ret == pdPASS) 
    {
        print_simple("Task 1 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }
    
    ret =  xTaskCreate(vTask2, "Task 2", 500, NULL, 2, NULL);
    if (ret == pdPASS)
    {
        print_simple("Task 2 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }

    ret =  xTaskCreate(vTask3, "Task 3", 500, NULL, 3, NULL);
    if (ret == pdPASS)
    {
        print_simple("Task 3 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }

    vTaskStartScheduler();

    while (1);
}