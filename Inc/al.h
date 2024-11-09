
#ifndef __ABSTRACTION_LAYER_INCLUDED__
#define __ABSTRACTION_LAYER_INCLUDED__

#ifdef __cplusplus
  extern "C"{
#endif //__cplusplus



/**
 * @def STM32Fx
 * @brief Defines the STM32 series.
 * @details Sets the target STM32 series (e.g., `f4`) for the library, 
 * which determines the corresponding header file inclusion.
 */
#define STM32Fx             f4

/**
 * @def AL_BUFFER_SIZE
 * @brief Defines the size of the tick buffer.
 * @details Specifies the number of tick jobs that can be managed concurrently.
 * Adjust based on system requirements (e.g., `5` jobs).
 */
#define AL_BUFFER_SIZE      5



/**
 * @brief Do not modify the following macros.
 * 
 * @warning Modifying these macros may break the library functionality.
 * These macros handle token concatenation and stringization for automatic 
 * inclusion of STM32 series-specific headers.
 */
#define __CONCAT(x, y)    x ## y
#define CONCAT(x, y)    __CONCAT(x, y)
#define __TO_STRING(x)  #x
#define TO_STRING(x)    __TO_STRING(x)

#include TO_STRING(CONCAT(CONCAT(stm32, STM32Fx), xx_ll_gpio.h))
#include TO_STRING(CONCAT(CONCAT(stm32, STM32Fx), xx_ll_bus.h))
#include <stdint.h>
#include <stdbool.h>




typedef void (*al_void_Fn) (void);

typedef struct {
  GPIO_TypeDef*  port;
  uint32_t       pin;
  uint32_t       mode;
  uint32_t       af;
  uint32_t       speed;
  uint32_t       pull;
  uint32_t       outType;
} vio_t;

#define  NO            LL_GPIO_AF_0,
#define  PIN(pin)      LL_GPIO_PIN_##pin,
#define  PORT(port)    GPIO##port,
#define  AF(af)        LL_GPIO_AF_##af,
#define  MODE(mode)    LL_GPIO_MODE_##mode,
#define  SPEED(speed)  LL_GPIO_SPEED_FREQ_##speed,
#define  TYPE(type)    LL_GPIO_OUTPUT_##type,
#define  PULL(pull)    LL_GPIO_PULL_##pull,
#define  LL_VIO(vio)   vio.port, vio.pin


bool al_createJob (al_void_Fn const callback, uint32_t tick_ms);
void al_doTheJob (void);
void al_tickIncrement (void);
uint32_t al_getTick (void);
bool vio_init (const vio_t* const PVIO, bool lock);

#ifdef __cplusplus
  };
#endif //__cplusplus

#endif //__ABSTRACTION_LAYER_INCLUDED__