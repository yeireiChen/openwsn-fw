//
// Created by randy2004 on 2017/9/19.
//

#include <headers/hw_memmap.h>
#include <headers/hw_types.h>
#include <headers/hw_gpio.h>
#include <headers/hw_ioc.h>
#include <source/gpio.h>
#include <source/ioc.h>
#include "dht_gpio.h"

#define GPIO_MASK           (0xfffff000)    /**< bit mask for GPIO port addr  */
#define PIN_MASK            (0x00000007)    /**< bit mask for GPIO pin [0-7]  */
#define IOC_PXX_OVER        0x400d4080

#define MODE_NOTSUP         (0x000000ff)

#define GPIO(pin) (gpio_t) (pin & GPIO_MASK)
#define GPIO_PIN_MASK(pin) (uint32_t)(1 << (pin & PIN_MASK))

int gpio_init(uint32_t pin, uint32_t mode ){
    /* check if mode is valid */
    if (mode == MODE_NOTSUP) {
        return -1;
    }

    /* disable any alternate function and any eventual interrupts */
    HWREG(GPIO(pin) + GPIO_O_IE) &= ~(GPIO_PIN_MASK(pin));
    HWREG(GPIO(pin) + GPIO_O_AFSEL) &= ~(GPIO_PIN_MASK(pin));



    /* configure pull configuration */
    //IOC_PXX_OVER[gpio_pp_num(pin)] = mode;
    IOCPadConfigSet(GPIO(pin),GPIO_PIN_MASK(pin),mode);
//    HWREG( 0x400d4080 ) = mode;

    //test
//    IOCPinConfigPeriphInput(GPIO(pin), GPIO_PIN_MASK(pin), IOC_GPT0OCP1);

    if (mode == IOC_OVERRIDE_OE) {
//        HWREG( (GPIO(pin) | GPIO_O_DIR)) |= GPIO_PIN_MASK(pin);
        HWREG(GPIO(pin) + GPIO_O_DIR) |= GPIO_PIN_MASK(pin);
    }
    else {
//        HWREG( (GPIO(pin) | GPIO_O_DIR)) &= ~GPIO_PIN_MASK(pin);
        HWREG(GPIO(pin) + GPIO_O_DIR) &= ~(GPIO_PIN_MASK(pin));
    }

    /* clear pin */
    //    HWREG( (GPIO(pin) | GPIO_O_DATA)) &= ~GPIO_PIN_MASK(pin);
//    HWREG(ui32Port + (GPIO_O_DATA + (ui8Pins << 2))) &= ~(ui8Pins);
    HWREG( (GPIO(pin) | GPIO_O_DATA) + (GPIO_DATA_DATA_M*4) ) &= ~(GPIO_PIN_MASK(pin));

    return 0;
}

void gpio_set(uint32_t pin){
//    HWREG( (GPIO(pin) | GPIO_O_DATA)) |= GPIO_PIN_MASK(pin);
//    HWREG(ui32Port + (GPIO_O_DATA + (ui8Pins << 2))) |= ui8Pins;
    HWREG( (GPIO(pin) | GPIO_O_DATA) + (GPIO_DATA_DATA_M*4) ) |= GPIO_PIN_MASK(pin);
}

void gpio_clear(uint32_t pin){
//    HWREG( (GPIO(pin) | GPIO_O_DATA)) &= ~GPIO_PIN_MASK(pin);
//    HWREG(ui32Port + (GPIO_O_DATA + (ui8Pins << 2))) &= ~(ui8Pins);
    HWREG( (GPIO(pin) | GPIO_O_DATA) + (GPIO_DATA_DATA_M*4) ) &= ~GPIO_PIN_MASK(pin);

}

int gpio_read(uint32_t pin){
//    return (int)( (GPIO(pin) | GPIO_O_DATA) & GPIO_PIN_MASK(pin) );
//    return (int)(HWREG(ui32Port + (GPIO_O_DATA + (ui8Pins << 2))));
//    return (int)( ((ui32Port | GPIO_O_DATA)  + (GPIO_DATA_DATA_M*32/8)) & ui8Pins );
    return (int)( HWREG( (GPIO(pin) | GPIO_O_DATA) + (GPIO_DATA_DATA_M*4)) & GPIO_PIN_MASK(pin) );
}

int gpio_readOut(void){
    return (int)( HWREG(IOC_GPT0OCP1));
}

