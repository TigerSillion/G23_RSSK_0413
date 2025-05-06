################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/rl78g16_filter_sample.c 

COMPILER_OBJS += \
src/rl78g16_filter_sample.obj 

C_DEPS += \
src/rl78g16_filter_sample.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrl -subcommand="src\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrl -subcommand="src\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"


