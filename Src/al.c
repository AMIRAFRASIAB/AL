
#include "al.h"

#ifndef NULL
  #define NULL  ((void*)0)
#endif    

static uint32_t __tickArray[AL_BUFFER_SIZE] = {0};
static al_void_Fn __tickCallback[AL_BUFFER_SIZE] = {NULL};
static uint32_t __tickCount = 0;

/**
 * @brief Get the index of the first available entry in the tick array.
 *
 * This function scans the tick array for the first available entry (with value 0).
 *
 * @return int32_t Index of the first available entry, or -1 if the array is full.
 *
 * @see AL_BUFFER_SIZE
 * 
 * Example usage:
 * @code
 * int32_t index = __al_getIndex();
 * if (index >= 0) {
 *   // Do something with the available index
 * }
 * @endcode
 */
static inline int32_t __al_getIndex (void) {
  for (uint32_t i = 0; i < AL_BUFFER_SIZE; i++) {
    if (__tickArray[i] == 0) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Enable the clock for a specific GPIO port.
 *
 * This function enables the clock for the GPIO port specified in the `PVIO` structure.
 *
 * @param[in] PVIO Pointer to the vio_t structure containing the port to be enabled.
 *
 * @see LL_AHB1_GRP1_EnableClock
 *
 * Example usage:
 * @code
 * const vio_t myVio = { GPIOA, ... };
 * __vio_clockEnable(&myVio);
 * @endcode
 */
static void __vio_clockEnable (const vio_t* const PVIO) {
  switch ((uintptr_t)(PVIO->port)) {
    #if defined(GPIOA)
    case (uintptr_t)GPIOA:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
      break;
    #endif
    #if defined(GPIOB)
    case (uintptr_t)GPIOB:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
      break;
    #endif
    #if defined(GPIOC)
    case (uintptr_t)GPIOC:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
      break;
    #endif
    #if defined(GPIOD)
    case (uintptr_t)GPIOD:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
      break;
    #endif
    #if defined(GPIOE)
    case (uintptr_t)GPIOE:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
      break;
    #endif
    #if defined(GPIOF)
    case (uintptr_t)GPIOF:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
      break;
    #endif
    #if defined(GPIOG)
    case (uintptr_t)GPIOG:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
      break;
    #endif
    #if defined(GPIOH)
    case (uintptr_t)GPIOH:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
      break;
    #endif
    #if defined(GPIOI)
    case (uintptr_t)GPIOI:
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
      break;
    #endif
  };
  __DSB();
}

/**
 * @brief Initialize a GPIO pin with the specified configuration.
 *
 * This function sets up the GPIO pin based on the `PVIO` configuration, enabling the clock,
 * setting mode, alternate function, pull-up/down, speed, output type, and optionally locking the configuration.
 *
 * @param[in] PVIO Pointer to the vio_t structure containing the GPIO configuration.
 * @param[in] lock Boolean indicating if the pin configuration should be locked.
 *
 * @return bool Status of the initialization. Returns `true` if the pin was successfully initialized.
 *
 * @see __vio_clockEnable
 *
 * Example usage:
 * @code
 * vio_t myVio = { GPIOA, LL_GPIO_MODE_OUTPUT, LL_GPIO_AF_0, LL_GPIO_PULL_NO, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_OUTPUT_PUSHPULL };
 * bool result = vio_init(&myVio, true);
 * @endcode
 */
bool vio_init (const vio_t* const PVIO, bool lock/*?*/) {
  __vio_clockEnable(PVIO);
  bool status = true;
  /* Mode */
  LL_GPIO_SetPinMode(LL_VIO((*PVIO)), PVIO->mode);
  __DSB();
  status = status && LL_GPIO_GetPinMode(LL_VIO((*PVIO))) == PVIO->mode;
  /* AF */
  if (PVIO->pin < LL_GPIO_PIN_8) {
    LL_GPIO_SetAFPin_0_7(LL_VIO((*PVIO)), PVIO->af);
    __DSB();
    status = status && LL_GPIO_GetAFPin_0_7(LL_VIO((*PVIO))) == PVIO->af;
  }
  else {
    LL_GPIO_SetAFPin_8_15(LL_VIO((*PVIO)), PVIO->af);
    __DSB();
    status = status && LL_GPIO_GetAFPin_8_15(LL_VIO((*PVIO))) == PVIO->af;
  }
  /* Pull */
  LL_GPIO_SetPinPull(LL_VIO((*PVIO)), PVIO->pull);
  __DSB();
  status = status && LL_GPIO_GetPinPull(LL_VIO((*PVIO))) == PVIO->pull;
  /* Speed */
  LL_GPIO_SetPinSpeed(LL_VIO((*PVIO)), PVIO->speed);
  __DSB();
  status = status && LL_GPIO_GetPinSpeed(LL_VIO((*PVIO))) == PVIO->speed;
  /* Output Type */
  LL_GPIO_SetPinOutputType(LL_VIO((*PVIO)), PVIO->outType);
  __DSB();
  status = status && LL_GPIO_GetPinOutputType(LL_VIO((*PVIO))) == PVIO->outType;
  /* Lock */
  if (lock == true) {
    LL_GPIO_LockPin(LL_VIO((*PVIO)));
    __DSB();
    status = status && LL_GPIO_IsPinLocked(LL_VIO((*PVIO)));
  }
  return status;
}

/**
 * @brief Create a job with a callback function to be executed after a specified delay.
 *
 * This function registers a callback function to be called after `tick_ms` ticks.
 *
 * @param[in] callback Function pointer to the job to be executed.
 * @param[in] tick_ms Delay in milliseconds before the job is executed.
 *
 * @return bool `true` if the job was successfully created, `false` if there was no available slot.
 *
 * @see __al_getIndex
 *
 * Example usage:
 * @code
 * al_createJob(myCallback, 1000); // Execute myCallback after 1000 ms
 * @endcode
 * @warning All callback function will execute in handler mode
 */
bool al_createJob (al_void_Fn const callback, uint32_t tick_ms) {
  int32_t index = __al_getIndex();
  if (index < 0) {
    return false;
  }
  index %= AL_BUFFER_SIZE;
  __tickArray[index] = tick_ms;
  __tickCallback[index] = callback;
  return true;
}

/**
 * @brief Increment the system tick counter.
 *
 * This function increments the global tick counter. It should be called regularly
 * to maintain timing consistency for jobs.
 *
 * Example usage:
 * @code
 * al_tickIncrement();
 * @endcode
 */
void al_tickIncrement (void) {
  __tickCount++;
}

/**
 * @brief Get the current value of the system tick counter.
 *
 * @return uint32_t The current tick count.
 *
 * Example usage:
 * @code
 * uint32_t ticks = al_getTick();
 * @endcode
 */
uint32_t al_getTick (void) {
  return __tickCount;
}

/**
 * @brief Execute registered jobs whose delay has expired.
 *
 * This function checks each job in the tick array, and if the tick counter for a job reaches 0,
 * the corresponding callback function is executed.
 *
 * Example usage:
 * @code
 * al_doTheJob();
 * @endcode
 * @attention Place this function in SysTick_Handler()
 */
void al_doTheJob (void) {
  uint32_t* tPtr = __tickArray;
  al_void_Fn* callPtr = __tickCallback; 
  uint32_t len = AL_BUFFER_SIZE;
  while (len-- > 0) {
    if (*tPtr != 0) {
      (*tPtr)--;
      if (*tPtr == 0 && *callPtr != NULL) {
        (*callPtr)();
      }
    }
    tPtr++;
    callPtr++;
  }
}
__INLINE void f (void) {
  __NOP();
}