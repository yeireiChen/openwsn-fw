//
// Created by randy2004 on 2017/9/19.
//

#ifndef __DHT_PARAMS_H__
#define __DHT_PARAMS_H__

#include "dht11.h"

/**
 * @brief   Set default configuration parameters for the DHT devices
 * @{
 */
#define IOC_OVERRIDE_PUE  0x00000004    /**< Pull Up Enable */
#define GPIO_IN_PU ((uint8_t)IOC_OVERRIDE_PUE)
#define GPIO_A_BASE 0x400D9000
#define PORTNUM_SHIFT       (12U)           /**< bit shift for GPIO port      */
#define GPIO_PIN(port, pin) (gpio_t)(((uint32_t)GPIO_A_BASE + \
                                      (port << PORTNUM_SHIFT)) | pin)


#ifndef DHT_PARAM_PIN
#define DHT_PARAM_PIN               (GPIO_PIN(0, 0))
#endif
#ifndef DHT_PARAM_TYPE
#define DHT_PARAM_TYPE              (DHT11)
#endif
#ifndef DHT_PARAM_PULL
#define DHT_PARAM_PULL              (GPIO_IN_PU)
#endif

#define DHT_PARAMS_DEFAULT          {.pin     = DHT_PARAM_PIN, \
                                     .type    = DHT_PARAM_TYPE, \
                                     .in_mode = DHT_PARAM_PULL}

/**
 * @brief   Configure DHT devices
 */
static const dht_params_t dht_params[] =
        {
//#ifdef DHT_PARAMS_BOARD
//                DHT_PARAMS_BOARD,
//#else
//                DHT_PARAMS_DEFAULT,
//#endif
                DHT_PARAMS_DEFAULT
        };



#endif //__DHT_PARAMS_H__