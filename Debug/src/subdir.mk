################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/IH2_IO2024.c \
../src/comm.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/dbg_printf_arm.c \
../src/ih2.c 

OBJS += \
./src/IH2_IO2024.o \
./src/comm.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/dbg_printf_arm.o \
./src/ih2.o 

C_DEPS += \
./src/IH2_IO2024.d \
./src/comm.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/dbg_printf_arm.d \
./src/ih2.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -DCR_INTEGER_PRINTF -D__LPC17XX__ -I"C:\Users\qubec\Documents\LPCXpresso_8.2.2_650\workspace\IH2_IO2024\inc" -I"C:\Users\qubec\Documents\LPCXpresso_8.2.2_650\workspace\CMSIS_CORE_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


