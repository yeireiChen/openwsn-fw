

#include <stdint.h>
#include <string.h>
#include "dht11.h"

#define US_PER_MS  (1000U)

//=========================== prototypes ======================================

void  wait2000Us_t(void);
void  wait20Us_t(void);
void  wait40_t(void);

//================================================================

static uint16_t read(uint32_t pin, int bits)
{
    uint16_t res = 0;

    for(int i=0;i< bits ;i++){
        uint32_t start, end;
        res <<= 1;
        /* measure the length between the next rising and falling flanks (the
            * time the pin is high - smoke up :-) */
        while (!gpio_read(pin)) {}
//        start = xtimer_now_usec();
        start = sctimer_readCounter();
//        while (gpio_read(pin)) {}
//        end = xtimer_now_usec();
        end = sctimer_readCounter();
        /* if the high phase was more than 40us = 1tick /ms, we got a 1 */
        if ((end - start) > 1) {
            res |= 0x0001;
        }
    }

    return res;
}

int dht_init(void)
{
    //#define IOC_OVERRIDE_OE   0x00000008    /**< Output Enable */
    gpio_init(dht_params.pin, 0x00000008);
    gpio_set(dht_params.pin);

    sctimer_set_callback(wait2000Us_t);
    sctimer_setCompare(sctimer_readCounter()+1);

    return DHT_OK;

}

int dht_read(void){

    uint8_t csum, sum;
    uint16_t raw_hum, raw_temp;

    /* send init signal to device */
    gpio_clear(dht_params.pin);
    
    sctimer_set_callback(wait20Us_t);
    sctimer_setCompare(sctimer_readCounter());

    gpio_set(dht_params.pin);
    
    sctimer_set_callback(wait40_t);
    sctimer_setCompare(sctimer_readCounter());


    /* sync on device */
    //IOC_OVERRIDE_PUE
    gpio_init(dht_params.pin,dht_params.in_mode);
    temperRature = (uint16_t)gpio_readOut();

    while (!gpio_read(dht_params.pin)) {}
//    while (gpio_read(dht_params.pin)) {}

    /*
     * data is read in sequentially, highest bit first:
     *  40 .. 24  23   ..   8  7  ..  0
     * [humidity][temperature][checksum]
     */

    /* read the humidity, temperature, and checksum bits */
//    raw_hum = read(dht_params.pin, 16);
//    raw_temp = read(dht_params.pin, 16);
//    csum = (uint8_t)read(dht_params.pin, 8);

    /* set pin high again - so we can trigger the next reading by pulling it low
     * again */
    //#define IOC_OVERRIDE_OE   0x00000008    /**< Output Enable */
    gpio_init(dht_params.pin,0x00000008);
    gpio_set(dht_params.pin);
//    gpio_init(dht_params.pin, GPIO_OUT);
//    gpio_set(dht_params.pin);

    /* validate the checksum */
//    sum = (raw_temp >> 8) + (raw_temp & 0xff) + (raw_hum >> 8) + (raw_hum & 0xff);
//    if ((sum != csum) || (csum == 0)) {
//        return DHT_NOCSUM;
//    }


    //set
//    humanity = raw_hum;
//    temperRature = raw_temp;

    /* parse the RAW values */
//    switch (dev.type) {
//        case DHT11:
//            temperRature = (int16_t)((raw_temp >> 8) * 10);
//            humanity = (int16_t)((raw_hum >> 8) * 10);
//            break;
//        default:
//            return DHT_NODEV;      /* this should never be reached */
//    }


    return DHT_OK;
}

uint16_t dht_readTemperature(void){

    if(dht_read()==DHT_OK){
        temperRature = 40;
    }
    else{
        temperRature = 200;
    }

    return temperRature;
}

uint16_t dht_readHumanity(void){

    return humanity;
}

// ========================== private =========================================

void  wait2000Us_t(void) {
    uint32_t wait_time = 2000;    //2000(us) * US_PER_MS=2000(ms)
    uint32_t compare_tick = sctimer_readCounter() + (wait_time*33);
    while(compare_tick - sctimer_readCounter() >0){}
}
void  wait20Us_t(void) {
    uint32_t wait_time = 20;    //20(us) * US_PER_MS=20(ms)
    uint32_t compare_tick = sctimer_readCounter() + (wait_time*33);
    while(compare_tick - sctimer_readCounter() >0){}
}
void  wait40_t(void) {
    uint32_t wait_time = 40;    //40us=0.04ms*33=1tick
    uint32_t compare_tick = sctimer_readCounter() + (1);
    while(compare_tick - sctimer_readCounter() >0){}
}
