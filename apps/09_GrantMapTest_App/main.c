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
#include <memory.h>
#include <xen/gntmap.h>
#include <xmalloc.h>

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

// pin numbers correspond to ODROID-XU3 header
#define GPIO_PIN_HEARTBEAT 10

// max grant number
#define MAX_GRANT_NUMBER 16


// grant map
static struct gntmap foreign_grants;



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

void vTaskGrantMap()
{
    error_t err;
    unsigned long host_addr;
    struct gntmap_entry *entry;


    gntmap_init(&foreign_grants);
    
    err = gntmap_set_max_grants(&foreign_grants, MAX_GRANT_NUMBER);

    if (!err)
    {
        printf("Allocated memory for grant table entries succesfully!\n");
    }
    else
    {
        printf("Memory allocation failed: %d", err);
        goto bail;
    }

    host_addr = (unsigned long)_xmalloc(4096, 4096);

    entry = gntmap_find_free_entry(&foreign_grants);

    err = gntmap_map_grant_ref(entry, host_addr, 0, 8, 0);

    if (!err)
    {
        printf("Mapped grant succesfully!\n");
    }
    else
    {
        printf("Grant map failed: %d\n", err);
        goto bail;
    }


bail:    

    while(1)
    {
        __asm("wfi");
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

    ret = xTaskCreate(vTaskGrantMap, "Task 2", 500, NULL, 2, NULL);
    
    vTaskStartScheduler();

    while(1)
    {
        
    }
}
