/**
 * @ brief:
 *
 * Simple FreeRTOS application.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <FreeRTOS.h>
#include <task.h>
#include <utils/print.h>

void vTask1()
{
    while(1)
    {
        print_simple("Task 1\n");
        vTaskDelay(1000);
    }
}

void vTask2()
{
    while(1)
    {
        print_simple("Task 2\n");
        vTaskDelay(2000);
    }
}

int main()
{
    print_simple("Entered main!\n");

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