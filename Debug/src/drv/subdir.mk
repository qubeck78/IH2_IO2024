################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drv/ih2_io.c \
../src/drv/lpc_uart.c 

OBJS += \
./src/drv/ih2_io.o \
./src/drv/lpc_uart.o 

C_DEPS += \
./src/drv/ih2_io.d \
./src/drv/lpc_uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/drv/%.o: ../src/drv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -DCR_INTEGER_PRINTF -D__LPC17XX__ -I"C:\Users\qubec\Documents\LPCXpresso_8.2.2_650\workspace\IH2_IO2024\inc" -I"C:\Users\qubec\Documents\LPCXpresso_8.2.2_650\workspace\CMSIS_CORE_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


