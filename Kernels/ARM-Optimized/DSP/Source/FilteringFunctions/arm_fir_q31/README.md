# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) with ARMv8.1M **Helium** assembly


Q.31 fixed-point FIR Filter processing function

This version allows boosting Q.31 FIR Filtering performance when using compilers having suboptimal Helium intrinsic code generation and can be sustitued to CMSIS DSP intrinsic variant.

[see API details](https://www.keil.com/pack/doc/CMSIS/DSP/html/group__FIR.html)

## BUILD

 - GCC 10.3-2021.10 example

```cpp
 arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -flax-vector-conversions -c arm_fir_q31.c -o arm_fir_q31.o -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include/
 ```

