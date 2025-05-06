################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../filter_sample/fir/fir_config_sample1.c \
../filter_sample/fir/r_ctsu_fir_sample.c 

COMPILER_OBJS += \
filter_sample/fir/fir_config_sample1.obj \
filter_sample/fir/r_ctsu_fir_sample.obj 

C_DEPS += \
filter_sample/fir/fir_config_sample1.d \
filter_sample/fir/r_ctsu_fir_sample.d 

# Each subdirectory must supply rules for building sources it contributes
filter_sample/fir/%.obj: ../filter_sample/fir/%.c 
	@echo 'Scanning and building file: $<'
	@echo filter_sample\fir\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\fir\cDepSubCommand.tmp"
	ccrl -subcommand="filter_sample\fir\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo filter_sample\fir\cSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\fir\cSubCommand.tmp"
	ccrl -subcommand="filter_sample\fir\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"


