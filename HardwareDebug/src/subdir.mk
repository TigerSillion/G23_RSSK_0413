################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/G23_RSSK_0412.c \
../src/slider_advanced_app.c \
../src/slider_advanced_main_example.c \
../src/slider_jump_main_example.c \
../src/slider_spiir_app.c \
../src/slider_spiir_main_example.c \
../src/slider_spiir_test.c 

COMPILER_OBJS += \
src/G23_RSSK_0412.obj \
src/slider_advanced_app.obj \
src/slider_advanced_main_example.obj \
src/slider_jump_main_example.obj \
src/slider_spiir_app.obj \
src/slider_spiir_main_example.obj \
src/slider_spiir_test.obj 

C_DEPS += \
src/G23_RSSK_0412.d \
src/slider_advanced_app.d \
src/slider_advanced_main_example.d \
src/slider_jump_main_example.d \
src/slider_spiir_app.d \
src/slider_spiir_main_example.d \
src/slider_spiir_test.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrl -subcommand="src\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=english "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrl -subcommand="src\cSubCommand.tmp" -msg_lang=english -o "$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

