################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
char_transciever.obj: ../char_transciever.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccsv7/tools/compiler/c5500_4.4.1/bin/cl55" -v5515 --memory_model=large -O2 --include_path="C:/Users/dell/Desktop/DSP_LAB/transceiver_file" --include_path="C:/ti/ccsv7/tools/compiler/c5500_4.4.1/include" --define=c5515 --display_error_number --diag_warning=225 --ptrdiff_size=16 --preproc_with_compile --preproc_dependency="char_transciever.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


