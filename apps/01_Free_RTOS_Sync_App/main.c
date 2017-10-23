/**
 * @ brief:
 *
 * Advanced FreeRTOS application.
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
xSemaphoreHandle xBinarySemaphore;
void vTask1()
{
    int i = 0;
    while(1)
    {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        print_register("Task 1", i++);
        xSemaphoreGive(xBinarySemaphore);
        vTaskDelay(500);
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
        vTaskDelay(1000);
    }
}
int main()
{
    print_simple("Entered main!\n");
    xBinarySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xBinarySemaphore);
    int ret = xTaskCreate(vTask1, "Task 1", 1000, NULL, 1, NULL);
    if (ret == pdPASS) 
    {
        print_simple("Task 1 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }
    ret =  xTaskCreate(vTask2, "Task 2", 1000, NULL, 2, NULL);
    if (ret == pdPASS)
    {
        print_simple("Task 2 succesfully created.\n");
    } 
    else
    {
        print_simple("Task not created.\n");
    }
    vTaskStartScheduler();
    while (1);
}