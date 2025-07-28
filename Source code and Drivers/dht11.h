#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "main.h"

// Define the GPIO Port and Pin for the DHT11 sensor
#define DHT11_PORT GPIOC
#define DHT11_PIN  GPIO_PIN_7

typedef enum
{
  DHT11_OK,
  DHT11_ERROR_CHECKSUM,
  DHT11_ERROR_RESPONSE,
  DHT11_ERROR_TIMEOUT
} DHT11_StatusTypeDef;

void DHT11_Init(TIM_HandleTypeDef *htim);
DHT11_StatusTypeDef DHT11_Read_Data(uint8_t *temperature, uint8_t *humidity);
void DHT11_Wakeup(void);

#endif /* INC_DHT11_H_ */
