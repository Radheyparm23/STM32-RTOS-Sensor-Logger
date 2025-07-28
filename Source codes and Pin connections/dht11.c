#include "dht11.h"
#include "cmsis_os.h"
#include <stdio.h>
static TIM_HandleTypeDef* dht_htim;
static const uint16_t dht_timeout = 1000;

static void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(dht_htim, 0);
    while (__HAL_TIM_GET_COUNTER(dht_htim) < us);
}

void DHT11_Init(TIM_HandleTypeDef *htim) {
    dht_htim = htim;
    HAL_TIM_Base_Start(dht_htim);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
}

void DHT11_Wakeup(void) {
    uint8_t temp, hum;
    printf("Waking up DHT11 sensor...\r\n");
    for (int i = 0; i < 5; i++) {
        if (DHT11_Read_Data(&temp, &hum) == DHT11_OK) {
            printf("DHT11 is awake! Temp=%dC, Hum=%d%%\r\n", temp, hum);
            return;
        }
        printf("...retry %d\r\n", i + 1);
        HAL_Delay(500);
    }
    printf("Warning: DHT11 did not respond to wakeup.\r\n");
}

DHT11_StatusTypeDef DHT11_Read_Data(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data_bits[5] = {0};
    uint16_t loop_counter;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Check if the scheduler is running
    UBaseType_t schedulerState = xTaskGetSchedulerState();

    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    delay_us(20000);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    // --- Enter Critical Section ONLY if scheduler is running ---
    if (schedulerState != taskSCHEDULER_NOT_STARTED) {
        taskENTER_CRITICAL();
    }

    loop_counter = dht_timeout;
    while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) if (--loop_counter == 0) { if(schedulerState != taskSCHEDULER_NOT_STARTED) taskEXIT_CRITICAL(); return DHT11_ERROR_TIMEOUT; }
    loop_counter = dht_timeout;
    while(!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) if (--loop_counter == 0) { if(schedulerState != taskSCHEDULER_NOT_STARTED) taskEXIT_CRITICAL(); return DHT11_ERROR_TIMEOUT; }
    loop_counter = dht_timeout;
    while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) if (--loop_counter == 0) { if(schedulerState != taskSCHEDULER_NOT_STARTED) taskEXIT_CRITICAL(); return DHT11_ERROR_TIMEOUT; }

    // --- Exit Critical Section ---
    if (schedulerState != taskSCHEDULER_NOT_STARTED) {
        taskEXIT_CRITICAL();
    }

    for (int i = 0; i < 40; i++) {
        loop_counter = dht_timeout;
        while(!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) if (--loop_counter == 0) return DHT11_ERROR_TIMEOUT;

        uint16_t high_pulse_time = 0;
        loop_counter = dht_timeout;
        while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
            high_pulse_time++;
            delay_us(1);
            if (--loop_counter == 0) return DHT11_ERROR_TIMEOUT;
        }
        if (high_pulse_time > 40) data_bits[i/8] |= (1 << (7 - (i % 8)));
    }

    uint8_t sum = data_bits[0] + data_bits[1] + data_bits[2] + data_bits[3];
    if (sum == data_bits[4]) {
        *humidity = data_bits[0];
        *temperature = data_bits[2];
        return DHT11_OK;
    }
    return DHT11_ERROR_CHECKSUM;
}
