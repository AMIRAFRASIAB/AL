
#ifndef __ABSTRACTION_LAYER_INCLUDED__
#define __ABSTRACTION_LAYER_INCLUDED__

#ifdef __cplusplus
  extern "C"{
#endif //__cplusplus

#include <stdint.h>
#include <stdbool.h>
#include "swo.h"

#define  PORT(port)                     CONCAT(GPIO, port)
#define  PIN(pin)                       CONCAT(LL_GPIO_PIN_, pin)
#define  MODE(mode)                     CONCAT(LL_GPIO_MODE_, mode)
#define  AF(af)                         CONCAT(LL_GPIO_AF_, af)
#define  SPEED(speed)                   CONCAT(LL_GPIO_SPEED_FREQ_, speed)
#define  PULL(pull)                     CONCAT(LL_GPIO_PULL_, pull)
#define  TYPE(type)                     CONCAT(LL_GPIO_OUTPUT_, type)


#define LL_GPIO_PULL_0                  LL_GPIO_PULL_NO

#define __AF_0                          LL_GPIO_SetAFPin_0_7
#define __AF_1                          LL_GPIO_SetAFPin_0_7
#define __AF_2                          LL_GPIO_SetAFPin_0_7
#define __AF_3                          LL_GPIO_SetAFPin_0_7
#define __AF_4                          LL_GPIO_SetAFPin_0_7
#define __AF_5                          LL_GPIO_SetAFPin_0_7
#define __AF_6                          LL_GPIO_SetAFPin_0_7
#define __AF_7                          LL_GPIO_SetAFPin_0_7
#define __AF_8                          LL_GPIO_SetAFPin_8_15
#define __AF_9                          LL_GPIO_SetAFPin_8_15
#define __AF_10                         LL_GPIO_SetAFPin_8_15
#define __AF_11                         LL_GPIO_SetAFPin_8_15
#define __AF_12                         LL_GPIO_SetAFPin_8_15
#define __AF_13                         LL_GPIO_SetAFPin_8_15
#define __AF_14                         LL_GPIO_SetAFPin_8_15
#define __AF_15                         LL_GPIO_SetAFPin_8_15



#define __VIO_H(_name, _port, _pin, _mode, _af, _speed, _pull, _type)                                                       \
                                        __attribute__((always_inline)) static inline void CONCAT(_name, _init) (void) {     \
                                          CONCAT(__HAL_RCC_GPIO, _port, _CLK_ENABLE)();                                     \
                                          LL_GPIO_SetPinMode(PORT(_port), PIN(_pin), MODE(_mode));                          \
                                          LL_GPIO_SetPinOutputType(PORT(_port), PIN(_pin), TYPE(_type));                    \
                                          LL_GPIO_SetPinSpeed(PORT(_port), PIN(_pin), SPEED(_speed));                       \
                                          LL_GPIO_SetPinPull(PORT(_port), PIN(_pin), PULL(_pull));                          \
                                          CONCAT(__AF_, _pin)(PORT(_port), PIN(_pin), AF(_af));                             \
                                          LL_GPIO_LockPin(PORT(_port), PIN(_pin));                                          \
                                        }                                                                                   \
                                        __attribute__((always_inline)) static inline uint32_t CONCAT(_name, _read) (void) { \
                                          return LL_GPIO_IsInputPinSet(PORT(_port), PIN(_pin));                             \
                                        }                                                                                   \
                                        __attribute__((always_inline)) static inline void CONCAT(_name, _set) (void) {      \
                                          LL_GPIO_SetOutputPin(PORT(_port), PIN(_pin));                                     \
                                        }                                                                                   \
                                        __attribute__((always_inline)) static inline void CONCAT(_name, _reset) (void) {    \
                                          LL_GPIO_ResetOutputPin(PORT(_port), PIN(_pin));                                   \
                                        }                                                                                   \
                                        __attribute__((always_inline)) static inline void CONCAT(_name, _toggle) (void) {   \
                                          LL_GPIO_TogglePin(PORT(_port), PIN(_pin));                                        \
                                        }



/**
 * @brief Generates a set of inline GPIO control functions for a specific pin.
 * 
 * This macro wraps the internal `__VIO_H` macro and simplifies GPIO peripheral access by
 * auto-generating a group of inline functions:
 * - <name>_init()     → Initializes the GPIO pin with the specified configuration.
 * - <name>_setPin()   → Sets the output pin to logical ON .
 * - <name>_resetPin() → Sets the output pin to logical OFF .
 * - <name>_getState() → Reads the pin input state and returns its current logic level.
 * 
 * @note All functions are `static inline` and consume **zero flash** if unused.
 * 
 * @usage
 * 
 * Define a pin with its parameters (in this order inside your some_file.h):
 * ```
 * <name>, <port>, <pin>, <mode>, <af>, <speed>, <pull>, <type>
 * ```
 * - `name`   : Logical name of the pin (e.g., `led`, `button`)
 * - `port`   : GPIO port letter (e.g., `A`, `B`, `C`)
 * - `pin`    : Pin number (e.g., `13`, `5`)
 * - `mode`   : GPIO mode (e.g., `OUTPUT`, `INPUT`, `ALTERNATE`)
 * - `af`     : Alternate function index (0 to 15)
 * - `speed`  : GPIO speed setting (`LOW`, `MEDIUM`, `HIGH`, `VERY_HIGH`)
 * - `pull`   : Pull-up/down config (`UP` or `DOWN`)
 * - `type`   : Output type (`PUSH_PULL` or `OPEN_DRAIN`)
 * 
 * @example
 * ```c
 * #define LED  led, A, 13, OUTPUT, 0, LOW, DOWN, PUSH_PULL
 * VIO_H(LED)
 * 
 * // Now you can use:
 * led_init();
 * led_enable();
 * led_disable();
 * if (led_getState() != 0) { ... }
 * ```
 */
#define VIO_H(...)   __VIO_H(__VA_ARGS__)





                                                    
#ifdef __cplusplus
  };
#endif //__cplusplus

#endif //__ABSTRACTION_LAYER_INCLUDED__