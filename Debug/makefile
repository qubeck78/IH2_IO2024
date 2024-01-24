################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/drv/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: IH2_IO2024.axf

# Tool invocations
IH2_IO2024.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\qubec\Documents\LPCXpresso_8.2.2_650\workspace\CMSIS_CORE_LPC17xx\Debug" -Xlinker -Map="IH2_IO2024.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "IH2_IO2024_Debug.ld" -o "IH2_IO2024.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) IH2_IO2024.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "IH2_IO2024.axf";  arm-none-eabi-objcopy -v -O binary "IH2_IO2024.axf" "IH2_IO2024.bin" ;  checksum -p LPC1763 -d "IH2_IO2024.bin";  arm-none-eabi-objcopy -I binary "IH2_IO2024.bin" -O ihex "IH2_IO2024.hex";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
