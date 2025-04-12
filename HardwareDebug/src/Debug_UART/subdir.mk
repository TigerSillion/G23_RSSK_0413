################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/Debug_UART/debug_uart.c 

COMPILER_OBJS += \
src/Debug_UART/debug_uart.obj 

C_DEPS += \
src/Debug_UART/debug_uart.d 

# Each subdirectory must supply rules for building sources it contributes
src/Debug_UART/%.obj: ../src/Debug_UART/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\Debug_UART\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\Debug_UART\cDepSubCommand.tmp"
	ccrl -subcommand="src\Debug_UART\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=english "$<"
	@echo src\Debug_UART\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\Debug_UART\cSubCommand.tmp"
	ccrl -subcommand="src\Debug_UART\cSubCommand.tmp" -msg_lang=english -o "$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

