#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO GPIO_NUM_2


void LED_Task(void *pvParameter)
{
    esp_rom_gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    uint8_t state = 0;
    while (1)
    {
        state = !state;
        gpio_set_level(LED_GPIO, state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(LED_Task, "LED Control Task", 2048, NULL, 1, NULL);
}



