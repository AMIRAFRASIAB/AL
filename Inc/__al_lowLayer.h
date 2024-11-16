
#ifndef __AL_LOW_LAYER_H_INCLUDED__
#define __AL_LOW_LAYER_H_INCLUDED__
#ifdef __cplusplus
  extern "C"
#endif  


#include "al.h"


#define __CONCAT(x, y)    x ## y
#define CONCAT(x, y)    __CONCAT(x, y)
#define __TO_STRING(x)  #x
#define TO_STRING(x)    __TO_STRING(x)

#define  PORT(port)    __CONCAT(GPIO, port)
#define  PIN(pin)      __CONCAT(LL_GPIO_PIN_, pin)
#define  MODE(mode)    __CONCAT(LL_GPIO_MODE_, mode)
#define  AF(af)        __CONCAT(LL_GPIO_AF_, af)
#define  SPEED(speed)  __CONCAT(LL_GPIO_SPEED_FREQ_, speed)
#define  PULL(pull)    __CONCAT(LL_GPIO_PULL_, pull)
#define  TYPE(type)    __CONCAT(LL_GPIO_OUTPUT_, type)
#define  LL_VIO(vio)   vio.port, vio.pin

#include TO_STRING(CONCAT(CONCAT(stm32, STM32Fx), xx_ll_gpio.h))
#include TO_STRING(CONCAT(CONCAT(stm32, STM32Fx), xx_ll_bus.h))
#include <stdint.h>
#include <stdbool.h>


#define __VIO_MAKE_OUTPUT_API_TOGGLE(vio)  void __CONCAT(vio, _toggle) (void) {          \
                                             LL_GPIO_TogglePin(LL_VIO(vio));             \
                                           }                                             \

#define __VIO_MAKE_OUTPUT_API_UP(vio)      void __CONCAT(vio, _off) (void) {             \
                                             LL_GPIO_SetOutputPin(LL_VIO(vio));          \
                                           }                                             \
                                           void __CONCAT(vio, _on) (void) {              \
                                             LL_GPIO_ResetOutputPin(LL_VIO(vio));        \
                                           }     
    
#define __VIO_MAKE_OUTPUT_API_DOWN(vio)    void __CONCAT(vio, _off) (void) {             \
                                             LL_GPIO_ResetOutputPin(LL_VIO(vio));        \
                                           }                                             \
                                           void __CONCAT(vio, _on) (void) {              \
                                             LL_GPIO_SetOutputPin(LL_VIO(vio));          \
                                           }     
    
#define __VIO_MAKE_INPUT_API_UP(vio)       bool __CONCAT(vio, _isEnabled) (void) {       \
                                             return LL_GPIO_IsInputPinSet(LL_VIO(vio));  \
                                           }                                             

#define __VIO_MAKE_INPUT_API_DOWN(vio)     bool __CONCAT(vio, _isEnabled) (void) {       \
                                             return !LL_GPIO_IsInputPinSet(LL_VIO(vio)); \
                                           }                                                                                             
                                                                                           
#define __VIO_INIT(vio)                    bool __CONCAT(vio, _init) (void) {            \
                                             return vio_init(&vio, true);                \
                                           }                                                                                           
                                           
/* INIT */
#define VIO_INIT(vio)                      __VIO_INIT(vio)
/* INPUT */                                
#define VIO_MAKE_INPUT_API_UP(vio)         __VIO_MAKE_INPUT_API_UP(vio)
#define VIO_MAKE_INPUT_API_DOWN(vio)       __VIO_MAKE_INPUT_API_DOWN(vio)
#define VIO_MAKE_INPUT_API_NO(...)         __VIO_MAKE_INPUT_API_UP(vio)
/* OUTPUT */                               
#define VIO_MAKE_OUTPUT_API_UP(vio)        __VIO_MAKE_OUTPUT_API_UP(vio)   __VIO_MAKE_OUTPUT_API_TOGGLE(vio)  __VIO_MAKE_INPUT_API_UP(vio)
#define VIO_MAKE_OUTPUT_API_DOWN(vio)      __VIO_MAKE_OUTPUT_API_DOWN(vio) __VIO_MAKE_OUTPUT_API_TOGGLE(vio)  __VIO_MAKE_INPUT_API_DOWN(vio)
#define VIO_MAKE_OUTPUT_API_NO(vio)        __VIO_MAKE_OUTPUT_API_UP(vio)   __VIO_MAKE_OUTPUT_API_TOGGLE(vio)  __VIO_MAKE_INPUT_API_UP(vio)
/* ANALOG */
#define VIO_MAKE_ANALOG_API_UP(...)        
#define VIO_MAKE_ANALOG_API_DOWN(...)      
#define VIO_MAKE_ANALOG_API_NO(...)        
/* ALTERNATE */                                           
#define VIO_MAKE_ALTERNATE_API_UP(...)
#define VIO_MAKE_ALTERNATE_API_DOWN(...)
#define VIO_MAKE_ALTERNATE_API_NO(...)
                                           
#ifdef __cplusplus
  };
#endif                                                                                
#endif //__AL_LOW_LAYER_H_INCLUDED__                                           