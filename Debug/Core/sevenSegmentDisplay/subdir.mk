################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/sevenSegmentDisplay/sevenSegmentDisplay.cpp 

OBJS += \
./Core/sevenSegmentDisplay/sevenSegmentDisplay.o 

CPP_DEPS += \
./Core/sevenSegmentDisplay/sevenSegmentDisplay.d 


# Each subdirectory must supply rules for building sources it contributes
Core/sevenSegmentDisplay/%.o Core/sevenSegmentDisplay/%.su: ../Core/sevenSegmentDisplay/%.cpp Core/sevenSegmentDisplay/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-sevenSegmentDisplay

clean-Core-2f-sevenSegmentDisplay:
	-$(RM) ./Core/sevenSegmentDisplay/sevenSegmentDisplay.d ./Core/sevenSegmentDisplay/sevenSegmentDisplay.o ./Core/sevenSegmentDisplay/sevenSegmentDisplay.su

.PHONY: clean-Core-2f-sevenSegmentDisplay

