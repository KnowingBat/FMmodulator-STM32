################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Application/Working/working.c 

OBJS += \
./Core/Application/Working/working.o 

C_DEPS += \
./Core/Application/Working/working.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Application/Working/%.o: ../Core/Application/Working/%.c Core/Application/Working/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I/home/davide/STM32CubeIDE/workspace_1.7.0/FMmodulator/Core/Inc/Application/Messages -I"C:/Users/David/STM32CubeIDE/workspace_1.8.0/FMmodulator-STM32/Core/Inc/Application/Working" -I"C:/Users/David/STM32CubeIDE/workspace_1.8.0/FMmodulator-STM32/Core/Inc/Application/Messages" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Application-2f-Working

clean-Core-2f-Application-2f-Working:
	-$(RM) ./Core/Application/Working/working.d ./Core/Application/Working/working.o

.PHONY: clean-Core-2f-Application-2f-Working

