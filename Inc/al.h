
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

#if AL_BUFFER_SIZE == 0
  #error "AL_BUFFER_SIZE Can't Be Zero"
#endif

#include "__al_lowLayer.h"

typedef void (*al_void_Fn) (void);
typedef uint32_t u32_t;

typedef struct {
  GPIO_TypeDef*  port;
  uint32_t       pin;
  uint32_t       mode;
  uint32_t       af;
  uint32_t       speed;
  uint32_t       pull;
  uint32_t       outType;
} vio_t;

                                           
bool  al_createJob (al_void_Fn const callback, uint32_t tick_ms);
void  al_doTheJob (void);
void  al_tickIncrement (void);
u32_t al_getTick (void);
bool  vio_init (const vio_t* const PVIO, bool lock);
/**
 * @brief Defines and initializes a virtual IO (VIO) structure with GPIO settings.
 * 
 * This macro creates a `vio_t` structure for a GPIO pin and generates functions for
 * controlling it (e.g., `name_on`, `name_off`, `name_toggle`). It configures the pin's 
 * port, mode, alternate function, speed, and pull-up/down resistors.
 * 
 * @param name    Name for the VIO structure and associated functions (e.g., `name_init`).
 * @param _port   GPIO port (e.g., A, B, C).
 * @param _pin    GPIO pin number (e.g., 0, 1, 2).
 * @param _mode   GPIO mode (INPUT, OUTPUT, ALTERNATE, ANALOG).
 * @param _af     Alternate function for the pin (0 if unused).
 * @param _speed  GPIO speed configuration (LOW, MEDIUM, HIGH).
 * @param _pull   Pull-up or pull-down resistor configuration (UP, DOWN, NO).
 * 
 * @details Declares and initializes the VIO structure, generating API functions 
 * based on the mode and pull configuration.
 * 
 * Example:
 * @code
 * VIO_C(led1, A, 5, OUTPUT, 0, HIGH, PULLUP);
 * @endcode
 */                   
#define VIO_C(params)    __VIO_C(params)
#define __VIO_C(name, _port, _pin, _mode, _af, _speed, _pull)                                             \
                                           const vio_t name = {                                           \
                                             .port   = PORT(_port)  ,                                     \
                                             .pin    = PIN(_pin)    ,                                     \
                                             .mode   = MODE(_mode)  ,                                     \
                                             .af     = AF(_af)      ,                                     \
                                             .speed  = SPEED(_speed),                                     \
                                             .pull   = PULL(_pull)                                        \
                                           };                                                             
                                           
/**
 * @brief Declares external references for a virtual IO (VIO) structure and its functions.
 * 
 * This macro declares a VIO structure and associated functions for GPIO control 
 * (e.g., `name_on`, `name_off`, `name_toggle`), typically used in header files to link 
 * to a VIO defined in another source file.
 * 
 * @param name Name of the VIO structure and its functions (e.g., `name_init`).
 * 
 * Example:
 * @code
 * // In header file (.h)
 * VIO_H(led1);
 * 
 * // In source file (.c)
 * VIO_C(led1, A, 5, OUTPUT, 0, HIGH, UP);
 * @endcode
 */     
#define VIO_H(params)    __VIO_H(params)
#define __VIO_H(name, _port, _pin, _mode, _af, _speed, _pull)                                             \
                                           extern const vio_t name;                                       \
                                           VIO_INIT(name);                                                \
                                           CONCAT(CONCAT(CONCAT(VIO_MAKE_, _mode), _API_), _pull)(name) 
                                           
                                                    
#ifdef __cplusplus
  };
#endif //__cplusplus

#endif //__ABSTRACTION_LAYER_INCLUDED__