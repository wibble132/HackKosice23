################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Src/ledControl.c \
../Application/Src/mainApp.c 

OBJS += \
./Application/Src/ledControl.o \
./Application/Src/mainApp.o 

C_DEPS += \
./Application/Src/ledControl.d \
./Application/Src/mainApp.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Src/%.o Application/Src/%.su Application/Src/%.cyclo: ../Application/Src/%.c Application/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/shark/OneDrive/Documents/Python/Hack Kosice 2023/HackKosice23/Drivers/BSP/LED12A1" -I"C:/Users/shark/OneDrive/Documents/Python/Hack Kosice 2023/HackKosice23/Drivers/BSP/Components/led1202" -I"C:/Users/shark/OneDrive/Documents/Python/Hack Kosice 2023/HackKosice23/Application/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-Src

clean-Application-2f-Src:
	-$(RM) ./Application/Src/ledControl.cyclo ./Application/Src/ledControl.d ./Application/Src/ledControl.o ./Application/Src/ledControl.su ./Application/Src/mainApp.cyclo ./Application/Src/mainApp.d ./Application/Src/mainApp.o ./Application/Src/mainApp.su

.PHONY: clean-Application-2f-Src

