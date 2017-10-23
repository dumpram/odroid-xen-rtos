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
// pin numbers correspond to ODROID-XU3 header
#define GPIO_PIN_OUT 10
#define GPIO_PIN_IN  24
xSemaphoreHandle xBinarySemaphore;
void vTask1()
{
    TickType_t lastWakeUp;
    gpio_init(GPIO_PIN_OUT, 1);
    gpio_set_value(GPIO_PIN_OUT, 0);
    lastWakeUp = xTaskGetTickCount();
    while(1)
    {
        vTaskDelayUntil(&lastWakeUp, 10);
        gpio_set_value(GPIO_PIN_OUT, 1);
        vTaskDelayUntil(&lastWakeUp, 10);
        gpio_set_value(GPIO_PIN_OUT, 0);
    }
}
void vTask2()
{
    int value;
    gpio_init(GPIO_PIN_IN, GPIO_MODE_IN);
    value = gpio_get_value(GPIO_PIN_IN);
    while(1)
    {
        while (value == gpio_get_value(GPIO_PIN_IN))
        {
            vTaskDelay(configTICK_RATE_HZ); // wait 1 second
            print_register("pin_value", value);
        }
        print_simple("State changed!\n");
        value = gpio_get_value(GPIO_PIN_IN);
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
    vTaskStartScheduler();
    while (1);
}