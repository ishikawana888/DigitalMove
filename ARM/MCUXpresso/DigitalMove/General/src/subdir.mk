################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bios.cpp \
../src/boot.cpp \
../src/cmd_act.cpp \
../src/holetbl.cpp \
../src/io.cpp \
../src/main.cpp \
../src/needle.cpp \
../src/sc0uart.cpp \
../src/sub_func.cpp \
../src/version.cpp 

OBJS += \
./src/bios.o \
./src/boot.o \
./src/cmd_act.o \
./src/holetbl.o \
./src/io.o \
./src/main.o \
./src/needle.o \
./src/sc0uart.o \
./src/sub_func.o \
./src/version.o 

CPP_DEPS += \
./src/bios.d \
./src/boot.d \
./src/cmd_act.d \
./src/holetbl.d \
./src/io.d \
./src/main.d \
./src/needle.d \
./src/sc0uart.d \
./src/sub_func.d \
./src/version.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -fno-builtin -Wall  -g -DTARGET_MBED_LPC1768 -DCALSELECT=0 -DTARGET_M3 -D__MBED_CMSIS_RTOS_CM -DDEVICE_CAN=1 -DDEVICE_I2CSLAVE=1 -DMBED_BUILD_TIMESTAMP=1593401688.5458272 -DTARGET_CORTEX_M -DDEVICE_LOCALFILESYSTEM=1 -DDEVICE_SERIAL=1 -DDEVICE_SERIAL_FC=1 -DTOOLCHAIN_GCC -DDEVICE_DEBUG_AWARENESS=1 -DDEVICE_INTERRUPTIN=1 -DDEVICE_ANALOGIN=1 -DDEVICE_STDIO_MESSAGES=1 -DTARGET_LIKE_CORTEX_M3 -D__MBED__=1 -DARM_MATH_CM3 -D__CMSIS_RTOS -DTARGET_NAME=LPC1768 -DDEVICE_PORTINOUT=1 -DTARGET_LPCTarget -DTARGET_RELEASE -DDEVICE_PORTOUT=1 -DDEVICE_SLEEP=1 -DDEVICE_SEMIHOST=1 -DDEVICE_PWMOUT=1 -DTARGET_LIKE_MBED -DDEVICE_ANALOGOUT=1 -DDEVICE_PORTIN=1 -DDEVICE_RTC=1 -DTOOLCHAIN_GCC_ARM -DTARGET_CORTEX -DTARGET_LPC1768 -D__CORTEX_M3 -DTARGET_LPC176X -DTARGET_NXP -DDEVICE_ETHERNET=1 -DDEVICE_SPISLAVE=1 -DDEVICE_ERROR_PATTERN=1 -DDEVICE_I2C=1 -DDEVICE_SPI=1 -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/" -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove//usr/src/mbed-sdk" -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/mbed" -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/mbed/TARGET_LPC1768" -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/src" -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/src/MODSERIAL" -I"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/src/TextLCD" -include"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/mbed_config.h" -std=gnu++98 -fabi-version=0 -fno-exceptions -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


