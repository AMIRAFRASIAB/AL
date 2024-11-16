
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
                                                    
#define VIO_C(name, _port, _pin, _mode, _af, _speed, _pull)                                               \
                                           const vio_t name = {                                           \
                                             .port   = PORT(_port)  ,                                     \
                                             .pin    = PIN(_pin)    ,                                     \
                                             .mode   = MODE(_mode)  ,                                     \
                                             .af     = AF(_af)      ,                                     \
                                             .speed  = SPEED(_speed),                                     \
                                             .pull   = PULL(_pull)                                        \
                                           };                                                             \
                                           VIO_INIT(name)                                                 \
                                           CONCAT(CONCAT(CONCAT(VIO_MAKE_, _mode), _API_), _pull)(name) 
                                                     
#define VIO_H(name)                        extern const vio_t name;                                       \
                                           bool CONCAT(name, _init) (void);                               \
                                           bool CONCAT(name, _isEnabled) (void);                          \
                                           void CONCAT(name, _on) (void);                                 \
                                           void CONCAT(name, _off) (void);                                \
                                           void CONCAT(name, _toggle) (void);
                                                    
#ifdef __cplusplus
  };
#endif //__cplusplus

#endif //__ABSTRACTION_LAYER_INCLUDED__