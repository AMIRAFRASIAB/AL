
# Abstraction Layer Library for STM32

This library provides an abstraction layer for handling GPIO initialization and periodic jobs using a SysTick timer on STM32 microcontrollers. The library allows you to manage multiple jobs that execute periodically at specified intervals.

## Features
- **GPIO Initialization**: A convenient way to configure GPIO pins on STM32 using the `vio_t` structure.
- **Job Scheduling**: The ability to create jobs that run periodically with the help of a tick buffer and the `al_doTheJob()` function.
- **SysTick Integration**: Designed to integrate with the SysTick interrupt for periodic task execution.

## Usage

### 1. Set the STM32 Series

```c
#define STM32Fx f4
```

### 2. Define the Buffer Size
Set the `AL_BUFFER_SIZE` macro to define the number of tick jobs that can be managed concurrently. Adjust this value based on your system requirements.

```c
#define AL_BUFFER_SIZE 5
```

### 3. Include the Library
After defining the necessary macros, include the abstraction layer header in your project.

```c
#include "al.h"
```

### 4. Initialize GPIO Pins
Use the `vio_init()` function to initialize GPIO pins. The `vio_t` structure is used to specify the GPIO port, pin, mode, speed, etc.

```c
vio_t vioPin = {GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT, LL_GPIO_AF_0, 
                LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO, LL_GPIO_OUTPUT_PUSHPULL};

vio_init(&vioPin, true);
```

### 5. Create Jobs
You can create periodic jobs using the `al_createJob()` function. This function allows you to define a callback and a tick interval for job execution.

```c
bool jobCreated = al_createJob(myCallbackFunction, 1000); // 1 second interval
```

### 6. Call `al_doTheJob()` in the SysTick ISR
The `al_doTheJob()` function should be placed inside the SysTick interrupt service routine (ISR) to ensure periodic execution of the scheduled jobs.

```c
void SysTick_Handler(void) {
  al_doTheJob();
  al_tickIncrement();
}
```

### 7. Increment the Tick
To increment the global tick counter, use the `al_tickIncrement()` function, which should also be called in the SysTick ISR.

```c
void SysTick_Handler(void) {
  al_tickIncrement();
  al_doTheJob();
}
```

### 8. Get the Current Tick Count
Use the `al_getTick()` function to retrieve the current tick count.

```c
uint32_t currentTick = al_getTick();
```



## Functions
- `bool vio_init(const vio_t* const PVIO, bool lock)`: Initializes a GPIO pin with the provided configuration.
- `bool al_createJob(al_void_Fn const callback, uint32_t tick_ms)`: Creates a job that will be executed periodically based on the provided tick interval.
- `void al_doTheJob(void)`: Executes all scheduled jobs if their tick interval has expired. **Note**: Once a job is executed, it is automatically removed from the job list.
- `void al_tickIncrement(void)`: Increments the global tick counter.
- `uint32_t al_getTick(void)`: Returns the current tick count.

## Job Execution and Deletion
- **Job Deletion After Execution**: Each time a job executes, it is removed from the list. This means that once a job is triggered and its callback function is executed, the job will not execute again unless it is re-added to the job list using `al_createJob()`.
  
- **Automatic Management**: The job list is automatically managed by the library, and completed jobs (i.e., those with a tick count of zero) are removed from the buffer to ensure that they don’t execute again.

## Notes
- **Do not modify the internal macros**: Modifying the token concatenation macros (`__CONCAT`, `CONCAT`, `__TO_STRING`, `TO_STRING`) may break the library functionality.
- **SysTick Timer**: The `al_doTheJob()` function must be called inside the SysTick ISR for proper job execution.

## Example

```c

#include "al.h"

void myCallbackFunction(void) {
  // Your periodic job code here
  al_createJob(myCallbackFunction, 1000);
}

int main(void) {
  // Initialize GPIO pin
  vio_t vioPin = {GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT, LL_GPIO_AF_0, 
                  LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO, LL_GPIO_OUTPUT_PUSHPULL};
  vio_init(&vioPin, true);

  // Create a job to run every 1 second
  al_createJob(myCallbackFunction, 1000);

  while (1) {
    // Main loop can do other tasks
  }
}

void SysTick_Handler(void) {
  al_doTheJob();        // Execute scheduled jobs
  al_tickIncrement();   // Increment the tick counter
}
```
