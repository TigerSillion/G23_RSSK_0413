################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../filter_sample/iir/iir_config_sample1.c \
../filter_sample/iir/r_ctsu_iir_sample.c 

COMPILER_OBJS += \
filter_sample/iir/iir_config_sample1.obj \
filter_sample/iir/r_ctsu_iir_sample.obj 

C_DEPS += \
filter_sample/iir/iir_config_sample1.d \
filter_sample/iir/r_ctsu_iir_sample.d 

# Each subdirectory must supply rules for building sources it contributes
filter_sample/iir/%.obj: ../filter_sample/iir/%.c 
	@echo 'Scanning and building file: $<'
	@echo filter_sample\iir\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\iir\cDepSubCommand.tmp"
	ccrl -subcommand="filter_sample\iir\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo filter_sample\iir\cSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\iir\cSubCommand.tmp"
	ccrl -subcommand="filter_sample\iir\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"


