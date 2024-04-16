################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
DigitalMoveNS30.srec \

SECONDARY_SIZE += \
DigitalMoveNS30.siz \


# All Target
all: DigitalMoveNS30.elf secondary-outputs

# Tool invocations
DigitalMoveNS30.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-builtin -Wall  -g -T "D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/Debug/linker-script-debug.ld" -Xlinker --gc-sections -L"D:/_ishikawa/Tools/DigitalMove/ARM/MCUXpresso/DigitalMove/mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM" -Wl,-Map,"DigitalMoveNS30.map" -mthumb -mcpu=cortex-m3 -mcpu=cortex-m3 -mthumb -Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -Wl,--end-group -o "DigitalMoveNS30.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DigitalMoveNS30.srec: DigitalMoveNS30.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O srec "DigitalMoveNS30.elf"  "DigitalMoveNS30.srec"
	@echo 'Finished building: $@'
	@echo ' '

DigitalMoveNS30.siz: DigitalMoveNS30.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "DigitalMoveNS30.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) DigitalMoveNS30.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
