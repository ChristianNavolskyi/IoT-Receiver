################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.5.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/navolskyi/workspace_v8/Project/adcbufcontinuous_CC1350_LAUNCHXL_TI" --include_path="/Users/navolskyi/workspace_v8/Project/adcbufcontinuous_CC1350_LAUNCHXL_TI" --include_path="/Users/navolskyi/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.5.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1859161855:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1859161855-inproc

build-1859161855-inproc: ../adcBufContinuousSampling.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/Users/navolskyi/ti/xdctools_3_32_00_06_core/xs" --xdcpath="/Users/navolskyi/ti/tirtos_cc13xx_cc26xx_2_21_00_06/packages;/Users/navolskyi/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages;/Users/navolskyi/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages;/Users/navolskyi/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/uia_2_01_00_01/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC1350F128 -r release -c "/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.5.LTS" --compileOptions "-mv7M3 --code_state=16 --float_support=vfplib -me --include_path=\"/Users/navolskyi/workspace_v8/Project/adcbufcontinuous_CC1350_LAUNCHXL_TI\" --include_path=\"/Users/navolskyi/workspace_v8/Project/adcbufcontinuous_CC1350_LAUNCHXL_TI\" --include_path=\"/Users/navolskyi/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272\" --include_path=\"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.5.LTS/include\" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1859161855 ../adcBufContinuousSampling.cfg
configPkg/compiler.opt: build-1859161855
configPkg/: build-1859161855


