################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_pincfg/Pin.c \
../src/smc_gen/r_pincfg/r_ctsu_pinset.c 

COMPILER_OBJS += \
src/smc_gen/r_pincfg/Pin.obj \
src/smc_gen/r_pincfg/r_ctsu_pinset.obj 

C_DEPS += \
src/smc_gen/r_pincfg/Pin.d \
src/smc_gen/r_pincfg/r_ctsu_pinset.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_pincfg/%.obj: ../src/smc_gen/r_pincfg/%.c 
	@echo 'Scanning and building file: $<'
	@echo src\smc_gen\r_pincfg\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\r_pincfg\cDepSubCommand.tmp"
	ccrl -subcommand="src\smc_gen\r_pincfg\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo src\smc_gen\r_pincfg\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\r_pincfg\cSubCommand.tmp"
	ccrl -subcommand="src\smc_gen\r_pincfg\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"


