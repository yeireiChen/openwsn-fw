//
// Created by randy2004 on 2017/9/19.
//

#ifndef __DHT11_H__
#define __DHT11_H__

#include <stdint.h>

#include "sctimer.h"
#include "dht_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_A_BASE 0x400D9000
#define PORTNUM_SHIFT       (12U)           /**< bit shift for GPIO port      */
#define GPIO_IN_PU ((uint8_t)IOC_OVERRIDE_PUE)
#define GPIO_PIN(port, pin) (gpio_t)(((uint32_t)GPIO_A_BASE + \
                                      (port << PORTNUM_SHIFT)) | pin)

/* @brief possible return codes
*/
enum {
    DHT_OK      =  0,       /**< all good */
    DHT_NOCSUM  = -1,       /**< checksum error */
    DHT_NODEV   = -2        /**< device type not defined */
};

/**
 * @brief data type for storing DHT sensor readings
 */
typedef struct {
    uint16_t humidity;      /**< relative deca-humidity */
    uint16_t temperature;   /**< temperature in deca-Celsius */
} dht_data_t;

/**
 * @brief device type of the DHT device
 */
typedef enum {
    DHT11,                  /**< DHT11 device identifier */
    DHT22,                  /**< DHT22 device identifier */
    DHT21 = DHT22           /**< DHT21 device identifier */
} dht_type_t;

/**
 * @brief device descriptor for DHT sensor devices
 */
typedef struct {
    gpio_t pin;             /**< GPIO pin of the device's data pin */
    dht_type_t type;        /**< type of the DHT device */
    gpio_mode_t in_mode;    /**< input pin configuration, with or without pull
                             *   resistor */
} dht_t;

/**
 * @brief configuration parameters for DHT devices
 */
typedef dht_t dht_params_t;

/**
 * @varialbe
 */

static uint32_t port = 0x400D9000;
static uint8_t pin = 0x01;
static dht_type_t type = DHT11;
//IOC_OVERRIDE_PUE  0x00000004  /**< Pull Up Enable */
static uint32_t mode =  0x00000004;

static uint16_t temperRature=0, humanity=0;
static uint32_t temperT;

static const dht_params_t dht_params ={
        GPIO_PIN(0, 0),
        DHT11,
        GPIO_IN_PU
};

/**
 * @brief initialize a new DHT device
 *
 * @param[out] dev      device descriptor of a DHT device
 * @param[in]  params   configuration parameters
 *
 * @return              0 on success
 * @return              -1 on error
 */
int dht_init(void);

/**
 * @brief   get a new temperature and humidity value from the device
 *
 * @note    if reading fails or checksum is invalid, no new values will be
 *          written into the result values
 *
 * @param[in]  dev      device descriptor of a DHT device
 * @param[out] temp     temperature value [in °C * 10^-1]
 * @param[out] hum      relative humidity value [in percent * 10^-1]
 *
 * @return              0 on success
 * @return              -1 on checksum error
 * @return              -2 on parsing error
 */
int dht_read(void);

uint16_t dht_readTemperature(void);
uint16_t dht_readHumanity(void);

#ifdef __cplusplus
}
#endif



#endif /* __DHT11_H__ */
