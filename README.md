
# STM32 GPIO Abstraction Layer (VIO_H)

This library provides a **zero-cost abstraction** over STM32 LL GPIO configuration using powerful preprocessor macros and `static inline` functions.

## ✨ Features

- No runtime overhead — all functions are inlined by the compiler
- Clean and scalable GPIO setup per pin
- Compile-time dispatching of alternate function (AF) registers
- Customizable through macro arguments
- Compatible with STM32 LL (Low Layer) drivers

## 🔧 Usage

### 1. Define a Pin

In your configuration header file:

```c
#define LED  led, A, 13, OUTPUT, 0, LOW, DOWN, PUSH_PULL
```

### 2. Generate Functions

```c
VIO_H(LED);
```

### 3. Use the Generated API

```c
led_init();
led_enable();
led_disable();

if (led_getState()) {
    // Do something if pin is high
}
```

## 🧠 API Generated

For a given name like `led`, these functions are generated:

- `led_init()` – Initializes the pin with clock, mode, speed, pull, type, AF
- `led_enable()` – Turns on the output pin (pull-dependent logic)
- `led_disable()` – Turns off the output pin
- `led_getState()` – Reads the pin input level

## ⚠️ Notes

- `_enable()` and `_disable()` logic depends on pull-up/pull-down config.
- Requires STM32 LL drivers (`LL_GPIO_*` functions).
- All macros and functions are resolved at compile-time.

## 📁 Integration

Include your macro definitions and the `abstraction_layer.h` header:

```c
#include "al.h"
```



---

Made with ❤️ for zero-cost embedded abstraction.
