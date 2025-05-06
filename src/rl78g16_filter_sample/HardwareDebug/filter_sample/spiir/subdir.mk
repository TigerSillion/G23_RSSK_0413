################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../filter_sample/spiir/r_ctsu_spiir_sample.c \
../filter_sample/spiir/spiir_config_sample.c 

COMPILER_OBJS += \
filter_sample/spiir/r_ctsu_spiir_sample.obj \
filter_sample/spiir/spiir_config_sample.obj 

C_DEPS += \
filter_sample/spiir/r_ctsu_spiir_sample.d \
filter_sample/spiir/spiir_config_sample.d 

# Each subdirectory must supply rules for building sources it contributes
filter_sample/spiir/%.obj: ../filter_sample/spiir/%.c 
	@echo 'Scanning and building file: $<'
	@echo filter_sample\spiir\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\spiir\cDepSubCommand.tmp"
	ccrl -subcommand="filter_sample\spiir\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo filter_sample\spiir\cSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\spiir\cSubCommand.tmp"
	ccrl -subcommand="filter_sample\spiir\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"


