################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../filter_sample/median/median_config_sample1.c \
../filter_sample/median/r_ctsu_median_sample.c 

COMPILER_OBJS += \
filter_sample/median/median_config_sample1.obj \
filter_sample/median/r_ctsu_median_sample.obj 

C_DEPS += \
filter_sample/median/median_config_sample1.d \
filter_sample/median/r_ctsu_median_sample.d 

# Each subdirectory must supply rules for building sources it contributes
filter_sample/median/%.obj: ../filter_sample/median/%.c 
	@echo 'Scanning and building file: $<'
	@echo filter_sample\median\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\median\cDepSubCommand.tmp"
	ccrl -subcommand="filter_sample\median\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo filter_sample\median\cSubCommand.tmp=
	@sed -e "s/^/ /" "filter_sample\median\cSubCommand.tmp"
	ccrl -subcommand="filter_sample\median\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"


