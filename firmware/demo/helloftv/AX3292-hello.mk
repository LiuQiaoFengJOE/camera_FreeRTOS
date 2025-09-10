##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-hello
ConfigurationName      :=Debug
WorkspacePath          := "E:\proj_code\WIFI_YG-K2T-TC\firmware\demo\helloftv"
ProjectPath            := "E:\proj_code\WIFI_YG-K2T-TC\firmware\demo\helloftv"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=11
Date                   :=4/9/2025
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=C:/MinGW-4.7.1/bin/or1k-elf-ld.exe
SharedObjectLinkerName :=C:/MinGW-4.7.1/bin/or1k-elf-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).elf
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="AX3292-hello.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  -T../../board/AX3292/mvl8801/AX3292.ld -Map=$(IntermediateDirectory)/$(ProjectName).map --gc-sections -( -l_lib8189ftvap32 -lAX3292-rtl8189ftv -lAX3292-sys -lAX3292-lib20 -lAX3292-libex -lAX3292-os -lAX3292-lwip -lAX3292-mcu -lc -lm -lgcc -lnosys -)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../inc $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../board/AX3292/mvl8801 $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../sys $(IncludeSwitch)../../lib $(IncludeSwitch)../../lib/drv $(IncludeSwitch)../../lib/mediaFile/mp3/inc $(IncludeSwitch)../../net $(IncludeSwitch)../../sys/drv $(IncludeSwitch)../../sys/drv/sensor/ $(IncludeSwitch)../../inc/net $(IncludeSwitch)../../arch/AX3292/hal/inc $(IncludeSwitch)../../arch/AX3292/ax32xx/inc $(IncludeSwitch)../../arch/AX3292/inc/asm $(IncludeSwitch)../device/tp 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)C:/MinGW-4.7.1/lib/gcc/or1k-elf/4.9.1/no-delay $(LibraryPathSwitch)C:/MinGW-4.7.1/or1k-elf/lib/no-delay  $(LibraryPathSwitch). $(LibraryPathSwitch)../../sdk $(LibraryPathSwitch)../../sdk/thirdparty 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-4.7.1/bin/or1k-elf-ar.exe rcu
CXX      := C:/MinGW-4.7.1/bin/or1k-elf-g++.exe
CC       := C:/MinGW-4.7.1/bin/or1k-elf-gcc.exe -DOR1K_NEWCPU -mno-delay
CXXFLAGS :=  -g -Os -Wall $(Preprocessors)
CFLAGS   :=  -g -Os -Wall -mhard-div -ffunction-sections -fdata-sections -lm $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.7.1/bin/or1k-elf-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/task_uiWin.c$(ObjectSuffix) $(IntermediateDirectory)/task_msgDeal.c$(ObjectSuffix) $(IntermediateDirectory)/task_msgSysRecommend.c$(ObjectSuffix) $(IntermediateDirectory)/task_msgSysUnshielded.c$(ObjectSuffix) $(IntermediateDirectory)/task_taskUI.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiTipsMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_taskScan.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcAudioPlayer.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcAudioPlayerRes.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_funcVideoRecord.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcVideoRecordMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcPlayBack.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcPlayBackMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcWifi.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcWifiMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_drvKey.c$(ObjectSuffix) $(IntermediateDirectory)/task_drvBattery.c$(ObjectSuffix) $(IntermediateDirectory)/task_font.c$(ObjectSuffix) $(IntermediateDirectory)/task_icon.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_config.c$(ObjectSuffix) $(IntermediateDirectory)/task_configGetValue.c$(ObjectSuffix) $(IntermediateDirectory)/task_configSystem.c$(ObjectSuffix) $(IntermediateDirectory)/task_menuMovieRec.c$(ObjectSuffix) $(IntermediateDirectory)/task_menuPlayBack.c$(ObjectSuffix) $(IntermediateDirectory)/task_toolFunc.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiAsternMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuFormatMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuItemMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuOptionMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuUsbMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuVersionMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiNoFileMsg.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_uiSelfTestMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiSlideShowMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiThumbnallMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiTips1Msg.c$(ObjectSuffix) $(IntermediateDirectory)/task_upgrade.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcPowerOff.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcUSBDev.c$(ObjectSuffix) $(IntermediateDirectory)/task_drvMisc.c$(ObjectSuffix) $(IntermediateDirectory)/task_mode.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_sysmem.c$(ObjectSuffix) $(IntermediateDirectory)/task_wifi_cfg.c$(ObjectSuffix) $(IntermediateDirectory)/task_main.c$(ObjectSuffix) $(IntermediateDirectory)/task_taskManage.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcUsbDevMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcTakePhoto.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcTakePhotoMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiUpgradeMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcAudioRecord.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcAudioRecordMsg.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_menuXml.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenuMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenuRes.c$(ObjectSuffix) $(IntermediateDirectory)/task_picSticker.c$(ObjectSuffix) $(IntermediateDirectory)/task_userMedia.c$(ObjectSuffix) $(IntermediateDirectory)/task_image_watermark.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcPhotoScoreRes.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcPhotoScore.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuIdContentRes.c$(ObjectSuffix) $(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix) $(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix) $(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix) \
	$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix) \
	

Objects1=$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix) $(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix) $(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix) $(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix) $(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix) $(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix) \
	$(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix) $(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) $(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix) $(IntermediateDirectory)/game_game_block.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix) $(IntermediateDirectory)/game_taskGame.c$(ObjectSuffix) $(IntermediateDirectory)/game_taskGameMenu.c$(ObjectSuffix) \
	$(IntermediateDirectory)/game_taskGameMenuMsg.c$(ObjectSuffix) $(IntermediateDirectory)/game_taskGameMenuRes.c$(ObjectSuffix) $(IntermediateDirectory)/game_taskGameMsg.c$(ObjectSuffix) $(IntermediateDirectory)/xos_xmsgq.c$(ObjectSuffix) $(IntermediateDirectory)/game_display.c$(ObjectSuffix) $(IntermediateDirectory)/xos_xos.c$(ObjectSuffix) $(IntermediateDirectory)/xos_x_cpu.c$(ObjectSuffix) $(IntermediateDirectory)/xos_xmbox.c$(ObjectSuffix) $(IntermediateDirectory)/xos_xwork.c$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	or1k-elf-ld.exe -o $(IntermediateDirectory)/AX3292-dbr.elf @"$(ProjectName).txt" -L../../sdk   -T../../board/AX3292/mvl8801/AX3292-dbr.ld --cref -Map=$(IntermediateDirectory)/AX3292-dbr.map --gc-sections -( -lAX3292-sys -lAX3292-mcu -)
	or1k-elf-objcopy -O binary $(IntermediateDirectory)/AX3292-dbr.elf $(IntermediateDirectory)/AX3292-dbr.bin
	or1k-elf-objcopy -O binary $(IntermediateDirectory)/$(ProjectName).elf $(IntermediateDirectory)/$(ProjectName).bin
	or1k-elf-size -A $(IntermediateDirectory)/$(ProjectName).elf
	..\..\output\boottool AX3292-Config.dat
	..\..\output\make.bat
	or1k-elf-objdump -S $(IntermediateDirectory)/AX3292-dbr.elf >$(IntermediateDirectory)/AX3292-dbr.lst
	or1k-elf-objdump -S $(IntermediateDirectory)/$(ProjectName).elf >$(IntermediateDirectory)/$(ProjectName).lst
	@echo Done

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/task_uiWin.c$(ObjectSuffix): task/uiWin.c $(IntermediateDirectory)/task_uiWin.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiWin.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiWin.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiWin.c$(DependSuffix): task/uiWin.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiWin.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiWin.c$(DependSuffix) -MM "task/uiWin.c"

$(IntermediateDirectory)/task_uiWin.c$(PreprocessSuffix): task/uiWin.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiWin.c$(PreprocessSuffix) "task/uiWin.c"

$(IntermediateDirectory)/task_msgDeal.c$(ObjectSuffix): task/msgDeal.c $(IntermediateDirectory)/task_msgDeal.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/msgDeal.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_msgDeal.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_msgDeal.c$(DependSuffix): task/msgDeal.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_msgDeal.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_msgDeal.c$(DependSuffix) -MM "task/msgDeal.c"

$(IntermediateDirectory)/task_msgDeal.c$(PreprocessSuffix): task/msgDeal.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_msgDeal.c$(PreprocessSuffix) "task/msgDeal.c"

$(IntermediateDirectory)/task_msgSysRecommend.c$(ObjectSuffix): task/msgSysRecommend.c $(IntermediateDirectory)/task_msgSysRecommend.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/msgSysRecommend.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_msgSysRecommend.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_msgSysRecommend.c$(DependSuffix): task/msgSysRecommend.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_msgSysRecommend.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_msgSysRecommend.c$(DependSuffix) -MM "task/msgSysRecommend.c"

$(IntermediateDirectory)/task_msgSysRecommend.c$(PreprocessSuffix): task/msgSysRecommend.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_msgSysRecommend.c$(PreprocessSuffix) "task/msgSysRecommend.c"

$(IntermediateDirectory)/task_msgSysUnshielded.c$(ObjectSuffix): task/msgSysUnshielded.c $(IntermediateDirectory)/task_msgSysUnshielded.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/msgSysUnshielded.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_msgSysUnshielded.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_msgSysUnshielded.c$(DependSuffix): task/msgSysUnshielded.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_msgSysUnshielded.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_msgSysUnshielded.c$(DependSuffix) -MM "task/msgSysUnshielded.c"

$(IntermediateDirectory)/task_msgSysUnshielded.c$(PreprocessSuffix): task/msgSysUnshielded.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_msgSysUnshielded.c$(PreprocessSuffix) "task/msgSysUnshielded.c"

$(IntermediateDirectory)/task_taskUI.c$(ObjectSuffix): task/taskUI.c $(IntermediateDirectory)/task_taskUI.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/taskUI.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_taskUI.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_taskUI.c$(DependSuffix): task/taskUI.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_taskUI.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_taskUI.c$(DependSuffix) -MM "task/taskUI.c"

$(IntermediateDirectory)/task_taskUI.c$(PreprocessSuffix): task/taskUI.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_taskUI.c$(PreprocessSuffix) "task/taskUI.c"

$(IntermediateDirectory)/task_uiTipsMsg.c$(ObjectSuffix): task/uiTipsMsg.c $(IntermediateDirectory)/task_uiTipsMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiTipsMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiTipsMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiTipsMsg.c$(DependSuffix): task/uiTipsMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiTipsMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiTipsMsg.c$(DependSuffix) -MM "task/uiTipsMsg.c"

$(IntermediateDirectory)/task_uiTipsMsg.c$(PreprocessSuffix): task/uiTipsMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiTipsMsg.c$(PreprocessSuffix) "task/uiTipsMsg.c"

$(IntermediateDirectory)/task_taskScan.c$(ObjectSuffix): task/taskScan.c $(IntermediateDirectory)/task_taskScan.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/taskScan.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_taskScan.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_taskScan.c$(DependSuffix): task/taskScan.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_taskScan.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_taskScan.c$(DependSuffix) -MM "task/taskScan.c"

$(IntermediateDirectory)/task_taskScan.c$(PreprocessSuffix): task/taskScan.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_taskScan.c$(PreprocessSuffix) "task/taskScan.c"

$(IntermediateDirectory)/task_funcAudioPlayer.c$(ObjectSuffix): task/funcAudioPlayer.c $(IntermediateDirectory)/task_funcAudioPlayer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcAudioPlayer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioPlayer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioPlayer.c$(DependSuffix): task/funcAudioPlayer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcAudioPlayer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcAudioPlayer.c$(DependSuffix) -MM "task/funcAudioPlayer.c"

$(IntermediateDirectory)/task_funcAudioPlayer.c$(PreprocessSuffix): task/funcAudioPlayer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioPlayer.c$(PreprocessSuffix) "task/funcAudioPlayer.c"

$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(ObjectSuffix): task/funcAudioPlayerMsg.c $(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcAudioPlayerMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(DependSuffix): task/funcAudioPlayerMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(DependSuffix) -MM "task/funcAudioPlayerMsg.c"

$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(PreprocessSuffix): task/funcAudioPlayerMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(PreprocessSuffix) "task/funcAudioPlayerMsg.c"

$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(ObjectSuffix): task/funcAudioPlayerRes.c $(IntermediateDirectory)/task_funcAudioPlayerRes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcAudioPlayerRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(DependSuffix): task/funcAudioPlayerRes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(DependSuffix) -MM "task/funcAudioPlayerRes.c"

$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(PreprocessSuffix): task/funcAudioPlayerRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioPlayerRes.c$(PreprocessSuffix) "task/funcAudioPlayerRes.c"

$(IntermediateDirectory)/task_funcVideoRecord.c$(ObjectSuffix): task/funcVideoRecord.c $(IntermediateDirectory)/task_funcVideoRecord.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcVideoRecord.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcVideoRecord.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcVideoRecord.c$(DependSuffix): task/funcVideoRecord.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcVideoRecord.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcVideoRecord.c$(DependSuffix) -MM "task/funcVideoRecord.c"

$(IntermediateDirectory)/task_funcVideoRecord.c$(PreprocessSuffix): task/funcVideoRecord.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcVideoRecord.c$(PreprocessSuffix) "task/funcVideoRecord.c"

$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(ObjectSuffix): task/funcVideoRecordMsg.c $(IntermediateDirectory)/task_funcVideoRecordMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcVideoRecordMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(DependSuffix): task/funcVideoRecordMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(DependSuffix) -MM "task/funcVideoRecordMsg.c"

$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(PreprocessSuffix): task/funcVideoRecordMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcVideoRecordMsg.c$(PreprocessSuffix) "task/funcVideoRecordMsg.c"

$(IntermediateDirectory)/task_funcPlayBack.c$(ObjectSuffix): task/funcPlayBack.c $(IntermediateDirectory)/task_funcPlayBack.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcPlayBack.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPlayBack.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPlayBack.c$(DependSuffix): task/funcPlayBack.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcPlayBack.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcPlayBack.c$(DependSuffix) -MM "task/funcPlayBack.c"

$(IntermediateDirectory)/task_funcPlayBack.c$(PreprocessSuffix): task/funcPlayBack.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPlayBack.c$(PreprocessSuffix) "task/funcPlayBack.c"

$(IntermediateDirectory)/task_funcPlayBackMsg.c$(ObjectSuffix): task/funcPlayBackMsg.c $(IntermediateDirectory)/task_funcPlayBackMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcPlayBackMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPlayBackMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPlayBackMsg.c$(DependSuffix): task/funcPlayBackMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcPlayBackMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcPlayBackMsg.c$(DependSuffix) -MM "task/funcPlayBackMsg.c"

$(IntermediateDirectory)/task_funcPlayBackMsg.c$(PreprocessSuffix): task/funcPlayBackMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPlayBackMsg.c$(PreprocessSuffix) "task/funcPlayBackMsg.c"

$(IntermediateDirectory)/task_funcWifi.c$(ObjectSuffix): task/funcWifi.c $(IntermediateDirectory)/task_funcWifi.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcWifi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcWifi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcWifi.c$(DependSuffix): task/funcWifi.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcWifi.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcWifi.c$(DependSuffix) -MM "task/funcWifi.c"

$(IntermediateDirectory)/task_funcWifi.c$(PreprocessSuffix): task/funcWifi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcWifi.c$(PreprocessSuffix) "task/funcWifi.c"

$(IntermediateDirectory)/task_funcWifiMsg.c$(ObjectSuffix): task/funcWifiMsg.c $(IntermediateDirectory)/task_funcWifiMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcWifiMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcWifiMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcWifiMsg.c$(DependSuffix): task/funcWifiMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcWifiMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcWifiMsg.c$(DependSuffix) -MM "task/funcWifiMsg.c"

$(IntermediateDirectory)/task_funcWifiMsg.c$(PreprocessSuffix): task/funcWifiMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcWifiMsg.c$(PreprocessSuffix) "task/funcWifiMsg.c"

$(IntermediateDirectory)/task_drvKey.c$(ObjectSuffix): task/drvKey.c $(IntermediateDirectory)/task_drvKey.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/drvKey.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_drvKey.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_drvKey.c$(DependSuffix): task/drvKey.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_drvKey.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_drvKey.c$(DependSuffix) -MM "task/drvKey.c"

$(IntermediateDirectory)/task_drvKey.c$(PreprocessSuffix): task/drvKey.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_drvKey.c$(PreprocessSuffix) "task/drvKey.c"

$(IntermediateDirectory)/task_drvBattery.c$(ObjectSuffix): task/drvBattery.c $(IntermediateDirectory)/task_drvBattery.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/drvBattery.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_drvBattery.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_drvBattery.c$(DependSuffix): task/drvBattery.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_drvBattery.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_drvBattery.c$(DependSuffix) -MM "task/drvBattery.c"

$(IntermediateDirectory)/task_drvBattery.c$(PreprocessSuffix): task/drvBattery.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_drvBattery.c$(PreprocessSuffix) "task/drvBattery.c"

$(IntermediateDirectory)/task_font.c$(ObjectSuffix): task/font.c $(IntermediateDirectory)/task_font.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/font.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_font.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_font.c$(DependSuffix): task/font.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_font.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_font.c$(DependSuffix) -MM "task/font.c"

$(IntermediateDirectory)/task_font.c$(PreprocessSuffix): task/font.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_font.c$(PreprocessSuffix) "task/font.c"

$(IntermediateDirectory)/task_icon.c$(ObjectSuffix): task/icon.c $(IntermediateDirectory)/task_icon.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/icon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_icon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_icon.c$(DependSuffix): task/icon.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_icon.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_icon.c$(DependSuffix) -MM "task/icon.c"

$(IntermediateDirectory)/task_icon.c$(PreprocessSuffix): task/icon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_icon.c$(PreprocessSuffix) "task/icon.c"

$(IntermediateDirectory)/task_config.c$(ObjectSuffix): task/config.c $(IntermediateDirectory)/task_config.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/config.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_config.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_config.c$(DependSuffix): task/config.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_config.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_config.c$(DependSuffix) -MM "task/config.c"

$(IntermediateDirectory)/task_config.c$(PreprocessSuffix): task/config.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_config.c$(PreprocessSuffix) "task/config.c"

$(IntermediateDirectory)/task_configGetValue.c$(ObjectSuffix): task/configGetValue.c $(IntermediateDirectory)/task_configGetValue.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/configGetValue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_configGetValue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_configGetValue.c$(DependSuffix): task/configGetValue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_configGetValue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_configGetValue.c$(DependSuffix) -MM "task/configGetValue.c"

$(IntermediateDirectory)/task_configGetValue.c$(PreprocessSuffix): task/configGetValue.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_configGetValue.c$(PreprocessSuffix) "task/configGetValue.c"

$(IntermediateDirectory)/task_configSystem.c$(ObjectSuffix): task/configSystem.c $(IntermediateDirectory)/task_configSystem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/configSystem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_configSystem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_configSystem.c$(DependSuffix): task/configSystem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_configSystem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_configSystem.c$(DependSuffix) -MM "task/configSystem.c"

$(IntermediateDirectory)/task_configSystem.c$(PreprocessSuffix): task/configSystem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_configSystem.c$(PreprocessSuffix) "task/configSystem.c"

$(IntermediateDirectory)/task_menuMovieRec.c$(ObjectSuffix): task/menuMovieRec.c $(IntermediateDirectory)/task_menuMovieRec.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/menuMovieRec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_menuMovieRec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_menuMovieRec.c$(DependSuffix): task/menuMovieRec.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_menuMovieRec.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_menuMovieRec.c$(DependSuffix) -MM "task/menuMovieRec.c"

$(IntermediateDirectory)/task_menuMovieRec.c$(PreprocessSuffix): task/menuMovieRec.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_menuMovieRec.c$(PreprocessSuffix) "task/menuMovieRec.c"

$(IntermediateDirectory)/task_menuPlayBack.c$(ObjectSuffix): task/menuPlayBack.c $(IntermediateDirectory)/task_menuPlayBack.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/menuPlayBack.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_menuPlayBack.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_menuPlayBack.c$(DependSuffix): task/menuPlayBack.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_menuPlayBack.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_menuPlayBack.c$(DependSuffix) -MM "task/menuPlayBack.c"

$(IntermediateDirectory)/task_menuPlayBack.c$(PreprocessSuffix): task/menuPlayBack.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_menuPlayBack.c$(PreprocessSuffix) "task/menuPlayBack.c"

$(IntermediateDirectory)/task_toolFunc.c$(ObjectSuffix): task/toolFunc.c $(IntermediateDirectory)/task_toolFunc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/toolFunc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_toolFunc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_toolFunc.c$(DependSuffix): task/toolFunc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_toolFunc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_toolFunc.c$(DependSuffix) -MM "task/toolFunc.c"

$(IntermediateDirectory)/task_toolFunc.c$(PreprocessSuffix): task/toolFunc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_toolFunc.c$(PreprocessSuffix) "task/toolFunc.c"

$(IntermediateDirectory)/task_uiAsternMsg.c$(ObjectSuffix): task/uiAsternMsg.c $(IntermediateDirectory)/task_uiAsternMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiAsternMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiAsternMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiAsternMsg.c$(DependSuffix): task/uiAsternMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiAsternMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiAsternMsg.c$(DependSuffix) -MM "task/uiAsternMsg.c"

$(IntermediateDirectory)/task_uiAsternMsg.c$(PreprocessSuffix): task/uiAsternMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiAsternMsg.c$(PreprocessSuffix) "task/uiAsternMsg.c"

$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(ObjectSuffix): task/uiMenuDateTime1Msg.c $(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuDateTime1Msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(DependSuffix): task/uiMenuDateTime1Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(DependSuffix) -MM "task/uiMenuDateTime1Msg.c"

$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(PreprocessSuffix): task/uiMenuDateTime1Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(PreprocessSuffix) "task/uiMenuDateTime1Msg.c"

$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(ObjectSuffix): task/uiMenuDefaultMsg.c $(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuDefaultMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(DependSuffix): task/uiMenuDefaultMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(DependSuffix) -MM "task/uiMenuDefaultMsg.c"

$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(PreprocessSuffix): task/uiMenuDefaultMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(PreprocessSuffix) "task/uiMenuDefaultMsg.c"

$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(ObjectSuffix): task/uiMenuDelAllMsg.c $(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuDelAllMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(DependSuffix): task/uiMenuDelAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(DependSuffix) -MM "task/uiMenuDelAllMsg.c"

$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(PreprocessSuffix): task/uiMenuDelAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(PreprocessSuffix) "task/uiMenuDelAllMsg.c"

$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(ObjectSuffix): task/uiMenuDelCurMsg.c $(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuDelCurMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(DependSuffix): task/uiMenuDelCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(DependSuffix) -MM "task/uiMenuDelCurMsg.c"

$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(PreprocessSuffix): task/uiMenuDelCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(PreprocessSuffix) "task/uiMenuDelCurMsg.c"

$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(ObjectSuffix): task/uiMenuFormatMsg.c $(IntermediateDirectory)/task_uiMenuFormatMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuFormatMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(DependSuffix): task/uiMenuFormatMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(DependSuffix) -MM "task/uiMenuFormatMsg.c"

$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(PreprocessSuffix): task/uiMenuFormatMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuFormatMsg.c$(PreprocessSuffix) "task/uiMenuFormatMsg.c"

$(IntermediateDirectory)/task_uiMenuItemMsg.c$(ObjectSuffix): task/uiMenuItemMsg.c $(IntermediateDirectory)/task_uiMenuItemMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuItemMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuItemMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuItemMsg.c$(DependSuffix): task/uiMenuItemMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuItemMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuItemMsg.c$(DependSuffix) -MM "task/uiMenuItemMsg.c"

$(IntermediateDirectory)/task_uiMenuItemMsg.c$(PreprocessSuffix): task/uiMenuItemMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuItemMsg.c$(PreprocessSuffix) "task/uiMenuItemMsg.c"

$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(ObjectSuffix): task/uiMenuLockCurMsg.c $(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuLockCurMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(DependSuffix): task/uiMenuLockCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(DependSuffix) -MM "task/uiMenuLockCurMsg.c"

$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(PreprocessSuffix): task/uiMenuLockCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(PreprocessSuffix) "task/uiMenuLockCurMsg.c"

$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(ObjectSuffix): task/uiMenuOptionMsg.c $(IntermediateDirectory)/task_uiMenuOptionMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuOptionMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(DependSuffix): task/uiMenuOptionMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(DependSuffix) -MM "task/uiMenuOptionMsg.c"

$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(PreprocessSuffix): task/uiMenuOptionMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuOptionMsg.c$(PreprocessSuffix) "task/uiMenuOptionMsg.c"

$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(ObjectSuffix): task/uiMenuUnlockAllMsg.c $(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuUnlockAllMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(DependSuffix): task/uiMenuUnlockAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(DependSuffix) -MM "task/uiMenuUnlockAllMsg.c"

$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(PreprocessSuffix): task/uiMenuUnlockAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(PreprocessSuffix) "task/uiMenuUnlockAllMsg.c"

$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(ObjectSuffix): task/uiMenuUnlockCurMsg.c $(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuUnlockCurMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(DependSuffix): task/uiMenuUnlockCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(DependSuffix) -MM "task/uiMenuUnlockCurMsg.c"

$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(PreprocessSuffix): task/uiMenuUnlockCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(PreprocessSuffix) "task/uiMenuUnlockCurMsg.c"

$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(ObjectSuffix): task/uiMenuUsbMsg.c $(IntermediateDirectory)/task_uiMenuUsbMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuUsbMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(DependSuffix): task/uiMenuUsbMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(DependSuffix) -MM "task/uiMenuUsbMsg.c"

$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(PreprocessSuffix): task/uiMenuUsbMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuUsbMsg.c$(PreprocessSuffix) "task/uiMenuUsbMsg.c"

$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(ObjectSuffix): task/uiMenuVersionMsg.c $(IntermediateDirectory)/task_uiMenuVersionMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuVersionMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(DependSuffix): task/uiMenuVersionMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(DependSuffix) -MM "task/uiMenuVersionMsg.c"

$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(PreprocessSuffix): task/uiMenuVersionMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuVersionMsg.c$(PreprocessSuffix) "task/uiMenuVersionMsg.c"

$(IntermediateDirectory)/task_uiNoFileMsg.c$(ObjectSuffix): task/uiNoFileMsg.c $(IntermediateDirectory)/task_uiNoFileMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiNoFileMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiNoFileMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiNoFileMsg.c$(DependSuffix): task/uiNoFileMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiNoFileMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiNoFileMsg.c$(DependSuffix) -MM "task/uiNoFileMsg.c"

$(IntermediateDirectory)/task_uiNoFileMsg.c$(PreprocessSuffix): task/uiNoFileMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiNoFileMsg.c$(PreprocessSuffix) "task/uiNoFileMsg.c"

$(IntermediateDirectory)/task_uiSelfTestMsg.c$(ObjectSuffix): task/uiSelfTestMsg.c $(IntermediateDirectory)/task_uiSelfTestMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiSelfTestMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiSelfTestMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiSelfTestMsg.c$(DependSuffix): task/uiSelfTestMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiSelfTestMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiSelfTestMsg.c$(DependSuffix) -MM "task/uiSelfTestMsg.c"

$(IntermediateDirectory)/task_uiSelfTestMsg.c$(PreprocessSuffix): task/uiSelfTestMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiSelfTestMsg.c$(PreprocessSuffix) "task/uiSelfTestMsg.c"

$(IntermediateDirectory)/task_uiSlideShowMsg.c$(ObjectSuffix): task/uiSlideShowMsg.c $(IntermediateDirectory)/task_uiSlideShowMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiSlideShowMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiSlideShowMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiSlideShowMsg.c$(DependSuffix): task/uiSlideShowMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiSlideShowMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiSlideShowMsg.c$(DependSuffix) -MM "task/uiSlideShowMsg.c"

$(IntermediateDirectory)/task_uiSlideShowMsg.c$(PreprocessSuffix): task/uiSlideShowMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiSlideShowMsg.c$(PreprocessSuffix) "task/uiSlideShowMsg.c"

$(IntermediateDirectory)/task_uiThumbnallMsg.c$(ObjectSuffix): task/uiThumbnallMsg.c $(IntermediateDirectory)/task_uiThumbnallMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiThumbnallMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiThumbnallMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiThumbnallMsg.c$(DependSuffix): task/uiThumbnallMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiThumbnallMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiThumbnallMsg.c$(DependSuffix) -MM "task/uiThumbnallMsg.c"

$(IntermediateDirectory)/task_uiThumbnallMsg.c$(PreprocessSuffix): task/uiThumbnallMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiThumbnallMsg.c$(PreprocessSuffix) "task/uiThumbnallMsg.c"

$(IntermediateDirectory)/task_uiTips1Msg.c$(ObjectSuffix): task/uiTips1Msg.c $(IntermediateDirectory)/task_uiTips1Msg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiTips1Msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiTips1Msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiTips1Msg.c$(DependSuffix): task/uiTips1Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiTips1Msg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiTips1Msg.c$(DependSuffix) -MM "task/uiTips1Msg.c"

$(IntermediateDirectory)/task_uiTips1Msg.c$(PreprocessSuffix): task/uiTips1Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiTips1Msg.c$(PreprocessSuffix) "task/uiTips1Msg.c"

$(IntermediateDirectory)/task_upgrade.c$(ObjectSuffix): task/upgrade.c $(IntermediateDirectory)/task_upgrade.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/upgrade.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_upgrade.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_upgrade.c$(DependSuffix): task/upgrade.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_upgrade.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_upgrade.c$(DependSuffix) -MM "task/upgrade.c"

$(IntermediateDirectory)/task_upgrade.c$(PreprocessSuffix): task/upgrade.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_upgrade.c$(PreprocessSuffix) "task/upgrade.c"

$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(ObjectSuffix): task/uiMenuLockAllMsg.c $(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuLockAllMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(DependSuffix): task/uiMenuLockAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(DependSuffix) -MM "task/uiMenuLockAllMsg.c"

$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(PreprocessSuffix): task/uiMenuLockAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(PreprocessSuffix) "task/uiMenuLockAllMsg.c"

$(IntermediateDirectory)/task_funcPowerOff.c$(ObjectSuffix): task/funcPowerOff.c $(IntermediateDirectory)/task_funcPowerOff.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcPowerOff.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPowerOff.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPowerOff.c$(DependSuffix): task/funcPowerOff.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcPowerOff.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcPowerOff.c$(DependSuffix) -MM "task/funcPowerOff.c"

$(IntermediateDirectory)/task_funcPowerOff.c$(PreprocessSuffix): task/funcPowerOff.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPowerOff.c$(PreprocessSuffix) "task/funcPowerOff.c"

$(IntermediateDirectory)/task_funcUSBDev.c$(ObjectSuffix): task/funcUSBDev.c $(IntermediateDirectory)/task_funcUSBDev.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcUSBDev.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcUSBDev.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcUSBDev.c$(DependSuffix): task/funcUSBDev.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcUSBDev.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcUSBDev.c$(DependSuffix) -MM "task/funcUSBDev.c"

$(IntermediateDirectory)/task_funcUSBDev.c$(PreprocessSuffix): task/funcUSBDev.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcUSBDev.c$(PreprocessSuffix) "task/funcUSBDev.c"

$(IntermediateDirectory)/task_drvMisc.c$(ObjectSuffix): task/drvMisc.c $(IntermediateDirectory)/task_drvMisc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/drvMisc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_drvMisc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_drvMisc.c$(DependSuffix): task/drvMisc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_drvMisc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_drvMisc.c$(DependSuffix) -MM "task/drvMisc.c"

$(IntermediateDirectory)/task_drvMisc.c$(PreprocessSuffix): task/drvMisc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_drvMisc.c$(PreprocessSuffix) "task/drvMisc.c"

$(IntermediateDirectory)/task_mode.c$(ObjectSuffix): task/mode.c $(IntermediateDirectory)/task_mode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/mode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_mode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_mode.c$(DependSuffix): task/mode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_mode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_mode.c$(DependSuffix) -MM "task/mode.c"

$(IntermediateDirectory)/task_mode.c$(PreprocessSuffix): task/mode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_mode.c$(PreprocessSuffix) "task/mode.c"

$(IntermediateDirectory)/task_sysmem.c$(ObjectSuffix): task/sysmem.c $(IntermediateDirectory)/task_sysmem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/sysmem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_sysmem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_sysmem.c$(DependSuffix): task/sysmem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_sysmem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_sysmem.c$(DependSuffix) -MM "task/sysmem.c"

$(IntermediateDirectory)/task_sysmem.c$(PreprocessSuffix): task/sysmem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_sysmem.c$(PreprocessSuffix) "task/sysmem.c"

$(IntermediateDirectory)/task_wifi_cfg.c$(ObjectSuffix): task/wifi_cfg.c $(IntermediateDirectory)/task_wifi_cfg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/wifi_cfg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_wifi_cfg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_wifi_cfg.c$(DependSuffix): task/wifi_cfg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_wifi_cfg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_wifi_cfg.c$(DependSuffix) -MM "task/wifi_cfg.c"

$(IntermediateDirectory)/task_wifi_cfg.c$(PreprocessSuffix): task/wifi_cfg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_wifi_cfg.c$(PreprocessSuffix) "task/wifi_cfg.c"

$(IntermediateDirectory)/task_main.c$(ObjectSuffix): task/main.c $(IntermediateDirectory)/task_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_main.c$(DependSuffix): task/main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_main.c$(DependSuffix) -MM "task/main.c"

$(IntermediateDirectory)/task_main.c$(PreprocessSuffix): task/main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_main.c$(PreprocessSuffix) "task/main.c"

$(IntermediateDirectory)/task_taskManage.c$(ObjectSuffix): task/taskManage.c $(IntermediateDirectory)/task_taskManage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/taskManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_taskManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_taskManage.c$(DependSuffix): task/taskManage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_taskManage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_taskManage.c$(DependSuffix) -MM "task/taskManage.c"

$(IntermediateDirectory)/task_taskManage.c$(PreprocessSuffix): task/taskManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_taskManage.c$(PreprocessSuffix) "task/taskManage.c"

$(IntermediateDirectory)/task_funcUsbDevMsg.c$(ObjectSuffix): task/funcUsbDevMsg.c $(IntermediateDirectory)/task_funcUsbDevMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcUsbDevMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcUsbDevMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcUsbDevMsg.c$(DependSuffix): task/funcUsbDevMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcUsbDevMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcUsbDevMsg.c$(DependSuffix) -MM "task/funcUsbDevMsg.c"

$(IntermediateDirectory)/task_funcUsbDevMsg.c$(PreprocessSuffix): task/funcUsbDevMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcUsbDevMsg.c$(PreprocessSuffix) "task/funcUsbDevMsg.c"

$(IntermediateDirectory)/task_funcTakePhoto.c$(ObjectSuffix): task/funcTakePhoto.c $(IntermediateDirectory)/task_funcTakePhoto.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcTakePhoto.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcTakePhoto.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcTakePhoto.c$(DependSuffix): task/funcTakePhoto.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcTakePhoto.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcTakePhoto.c$(DependSuffix) -MM "task/funcTakePhoto.c"

$(IntermediateDirectory)/task_funcTakePhoto.c$(PreprocessSuffix): task/funcTakePhoto.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcTakePhoto.c$(PreprocessSuffix) "task/funcTakePhoto.c"

$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(ObjectSuffix): task/funcTakePhotoMsg.c $(IntermediateDirectory)/task_funcTakePhotoMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcTakePhotoMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(DependSuffix): task/funcTakePhotoMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(DependSuffix) -MM "task/funcTakePhotoMsg.c"

$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(PreprocessSuffix): task/funcTakePhotoMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcTakePhotoMsg.c$(PreprocessSuffix) "task/funcTakePhotoMsg.c"

$(IntermediateDirectory)/task_uiUpgradeMsg.c$(ObjectSuffix): task/uiUpgradeMsg.c $(IntermediateDirectory)/task_uiUpgradeMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiUpgradeMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiUpgradeMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiUpgradeMsg.c$(DependSuffix): task/uiUpgradeMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiUpgradeMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiUpgradeMsg.c$(DependSuffix) -MM "task/uiUpgradeMsg.c"

$(IntermediateDirectory)/task_uiUpgradeMsg.c$(PreprocessSuffix): task/uiUpgradeMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiUpgradeMsg.c$(PreprocessSuffix) "task/uiUpgradeMsg.c"

$(IntermediateDirectory)/task_funcAudioRecord.c$(ObjectSuffix): task/funcAudioRecord.c $(IntermediateDirectory)/task_funcAudioRecord.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcAudioRecord.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioRecord.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioRecord.c$(DependSuffix): task/funcAudioRecord.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcAudioRecord.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcAudioRecord.c$(DependSuffix) -MM "task/funcAudioRecord.c"

$(IntermediateDirectory)/task_funcAudioRecord.c$(PreprocessSuffix): task/funcAudioRecord.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioRecord.c$(PreprocessSuffix) "task/funcAudioRecord.c"

$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(ObjectSuffix): task/funcAudioRecordMsg.c $(IntermediateDirectory)/task_funcAudioRecordMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcAudioRecordMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(DependSuffix): task/funcAudioRecordMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(DependSuffix) -MM "task/funcAudioRecordMsg.c"

$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(PreprocessSuffix): task/funcAudioRecordMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioRecordMsg.c$(PreprocessSuffix) "task/funcAudioRecordMsg.c"

$(IntermediateDirectory)/task_menuXml.c$(ObjectSuffix): task/menuXml.c $(IntermediateDirectory)/task_menuXml.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/menuXml.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_menuXml.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_menuXml.c$(DependSuffix): task/menuXml.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_menuXml.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_menuXml.c$(DependSuffix) -MM "task/menuXml.c"

$(IntermediateDirectory)/task_menuXml.c$(PreprocessSuffix): task/menuXml.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_menuXml.c$(PreprocessSuffix) "task/menuXml.c"

$(IntermediateDirectory)/task_funMainMenu.c$(ObjectSuffix): task/funMainMenu.c $(IntermediateDirectory)/task_funMainMenu.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funMainMenu.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu.c$(DependSuffix): task/funMainMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funMainMenu.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funMainMenu.c$(DependSuffix) -MM "task/funMainMenu.c"

$(IntermediateDirectory)/task_funMainMenu.c$(PreprocessSuffix): task/funMainMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu.c$(PreprocessSuffix) "task/funMainMenu.c"

$(IntermediateDirectory)/task_funMainMenuMsg.c$(ObjectSuffix): task/funMainMenuMsg.c $(IntermediateDirectory)/task_funMainMenuMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funMainMenuMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenuMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenuMsg.c$(DependSuffix): task/funMainMenuMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funMainMenuMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funMainMenuMsg.c$(DependSuffix) -MM "task/funMainMenuMsg.c"

$(IntermediateDirectory)/task_funMainMenuMsg.c$(PreprocessSuffix): task/funMainMenuMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenuMsg.c$(PreprocessSuffix) "task/funMainMenuMsg.c"

$(IntermediateDirectory)/task_funMainMenuRes.c$(ObjectSuffix): task/funMainMenuRes.c $(IntermediateDirectory)/task_funMainMenuRes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funMainMenuRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenuRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenuRes.c$(DependSuffix): task/funMainMenuRes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funMainMenuRes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funMainMenuRes.c$(DependSuffix) -MM "task/funMainMenuRes.c"

$(IntermediateDirectory)/task_funMainMenuRes.c$(PreprocessSuffix): task/funMainMenuRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenuRes.c$(PreprocessSuffix) "task/funMainMenuRes.c"

$(IntermediateDirectory)/task_picSticker.c$(ObjectSuffix): task/picSticker.c $(IntermediateDirectory)/task_picSticker.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/picSticker.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_picSticker.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_picSticker.c$(DependSuffix): task/picSticker.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_picSticker.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_picSticker.c$(DependSuffix) -MM "task/picSticker.c"

$(IntermediateDirectory)/task_picSticker.c$(PreprocessSuffix): task/picSticker.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_picSticker.c$(PreprocessSuffix) "task/picSticker.c"

$(IntermediateDirectory)/task_userMedia.c$(ObjectSuffix): task/userMedia.c $(IntermediateDirectory)/task_userMedia.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/userMedia.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_userMedia.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_userMedia.c$(DependSuffix): task/userMedia.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_userMedia.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_userMedia.c$(DependSuffix) -MM "task/userMedia.c"

$(IntermediateDirectory)/task_userMedia.c$(PreprocessSuffix): task/userMedia.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_userMedia.c$(PreprocessSuffix) "task/userMedia.c"

$(IntermediateDirectory)/task_image_watermark.c$(ObjectSuffix): task/image_watermark.c $(IntermediateDirectory)/task_image_watermark.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/image_watermark.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_image_watermark.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_image_watermark.c$(DependSuffix): task/image_watermark.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_image_watermark.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_image_watermark.c$(DependSuffix) -MM "task/image_watermark.c"

$(IntermediateDirectory)/task_image_watermark.c$(PreprocessSuffix): task/image_watermark.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_image_watermark.c$(PreprocessSuffix) "task/image_watermark.c"

$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(ObjectSuffix): task/funcPhotoScoreMsg.c $(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcPhotoScoreMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(DependSuffix): task/funcPhotoScoreMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(DependSuffix) -MM "task/funcPhotoScoreMsg.c"

$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(PreprocessSuffix): task/funcPhotoScoreMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(PreprocessSuffix) "task/funcPhotoScoreMsg.c"

$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(ObjectSuffix): task/funcPhotoScoreRes.c $(IntermediateDirectory)/task_funcPhotoScoreRes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcPhotoScoreRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(DependSuffix): task/funcPhotoScoreRes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(DependSuffix) -MM "task/funcPhotoScoreRes.c"

$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(PreprocessSuffix): task/funcPhotoScoreRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPhotoScoreRes.c$(PreprocessSuffix) "task/funcPhotoScoreRes.c"

$(IntermediateDirectory)/task_funcPhotoScore.c$(ObjectSuffix): task/funcPhotoScore.c $(IntermediateDirectory)/task_funcPhotoScore.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/funcPhotoScore.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPhotoScore.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPhotoScore.c$(DependSuffix): task/funcPhotoScore.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_funcPhotoScore.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_funcPhotoScore.c$(DependSuffix) -MM "task/funcPhotoScore.c"

$(IntermediateDirectory)/task_funcPhotoScore.c$(PreprocessSuffix): task/funcPhotoScore.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPhotoScore.c$(PreprocessSuffix) "task/funcPhotoScore.c"

$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(ObjectSuffix): task/uiMenuIdContentMsg.c $(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuIdContentMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(DependSuffix): task/uiMenuIdContentMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(DependSuffix) -MM "task/uiMenuIdContentMsg.c"

$(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(PreprocessSuffix): task/uiMenuIdContentMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuIdContentMsg.c$(PreprocessSuffix) "task/uiMenuIdContentMsg.c"

$(IntermediateDirectory)/task_uiMenuIdContentRes.c$(ObjectSuffix): task/uiMenuIdContentRes.c $(IntermediateDirectory)/task_uiMenuIdContentRes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/uiMenuIdContentRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuIdContentRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuIdContentRes.c$(DependSuffix): task/uiMenuIdContentRes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_uiMenuIdContentRes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_uiMenuIdContentRes.c$(DependSuffix) -MM "task/uiMenuIdContentRes.c"

$(IntermediateDirectory)/task_uiMenuIdContentRes.c$(PreprocessSuffix): task/uiMenuIdContentRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuIdContentRes.c$(PreprocessSuffix) "task/uiMenuIdContentRes.c"

$(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix): task/sd_config.c $(IntermediateDirectory)/task_sd_config.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/sd_config.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_sd_config.c$(DependSuffix): task/sd_config.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix) -MF$(IntermediateDirectory)/task_sd_config.c$(DependSuffix) -MM "task/sd_config.c"

$(IntermediateDirectory)/task_sd_config.c$(PreprocessSuffix): task/sd_config.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_sd_config.c$(PreprocessSuffix) "task/sd_config.c"

$(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix): resource/user_icon.c $(IntermediateDirectory)/resource_user_icon.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/resource/user_icon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/resource_user_icon.c$(DependSuffix): resource/user_icon.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix) -MF$(IntermediateDirectory)/resource_user_icon.c$(DependSuffix) -MM "resource/user_icon.c"

$(IntermediateDirectory)/resource_user_icon.c$(PreprocessSuffix): resource/user_icon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/resource_user_icon.c$(PreprocessSuffix) "resource/user_icon.c"

$(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix): resource/user_str.c $(IntermediateDirectory)/resource_user_str.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/resource/user_str.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/resource_user_str.c$(DependSuffix): resource/user_str.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix) -MF$(IntermediateDirectory)/resource_user_str.c$(DependSuffix) -MM "resource/user_str.c"

$(IntermediateDirectory)/resource_user_str.c$(PreprocessSuffix): resource/user_str.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/resource_user_str.c$(PreprocessSuffix) "resource/user_str.c"

$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix): ../device/lcd/lcd_mcu_5420.c $(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_5420.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(DependSuffix): ../device/lcd/lcd_mcu_5420.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_5420.c"

$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_5420.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_5420.c"

$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix): ../device/lcd/lcd_mcu_9225G.c $(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_9225G.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(DependSuffix): ../device/lcd/lcd_mcu_9225G.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_9225G.c"

$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_9225G.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_9225G.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix): ../device/lcd/lcd_mcu_hx8352b.c $(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_hx8352b.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(DependSuffix): ../device/lcd/lcd_mcu_hx8352b.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_hx8352b.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_hx8352b.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_hx8352b.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix): ../device/lcd/lcd_mcu_hx8352c.c $(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_hx8352c.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(DependSuffix): ../device/lcd/lcd_mcu_hx8352c.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_hx8352c.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_hx8352c.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_hx8352c.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix): ../device/lcd/lcd_mcu_hx8357b.c $(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_hx8357b.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(DependSuffix): ../device/lcd/lcd_mcu_hx8357b.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_hx8357b.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_hx8357b.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_hx8357b.c"

$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix): ../device/lcd/lcd_mcu_ili9328.c $(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_ili9328.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(DependSuffix): ../device/lcd/lcd_mcu_ili9328.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_ili9328.c"

$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_ili9328.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_ili9328.c"

$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix): ../device/lcd/lcd_mcu_lgdp4532.c $(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_lgdp4532.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(DependSuffix): ../device/lcd/lcd_mcu_lgdp4532.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_lgdp4532.c"

$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_lgdp4532.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_lgdp4532.c"

$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix): ../device/lcd/lcd_mcu_r61509v.c $(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_r61509v.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(DependSuffix): ../device/lcd/lcd_mcu_r61509v.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_r61509v.c"

$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_r61509v.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_r61509v.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7789.c $(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_st7789.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(DependSuffix): ../device/lcd/lcd_mcu_st7789.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_st7789.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7789.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7789.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7789v.c $(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_st7789v.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(DependSuffix): ../device/lcd/lcd_mcu_st7789v.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_st7789v.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7789v.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7789v.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ili8961.c $(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_ili8961.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(DependSuffix): ../device/lcd/lcd_rgb_ili8961.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_ili8961.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ili8961.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ili8961.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ili9342c.c $(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_ili9342c.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(DependSuffix): ../device/lcd/lcd_rgb_ili9342c.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_ili9342c.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ili9342c.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ili9342c.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ili9806e.c $(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_ili9806e.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(DependSuffix): ../device/lcd/lcd_rgb_ili9806e.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_ili9806e.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ili9806e.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ili9806e.c"

$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix): ../device/lcd/lcd_rgb_jl_lcd.c $(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_jl_lcd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(DependSuffix): ../device/lcd/lcd_rgb_jl_lcd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_jl_lcd.c"

$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_jl_lcd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_jl_lcd.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ota5182.c $(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_ota5182.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(DependSuffix): ../device/lcd/lcd_rgb_ota5182.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_ota5182.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ota5182.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ota5182.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7282.c $(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_st7282.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(DependSuffix): ../device/lcd/lcd_rgb_st7282.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_st7282.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7282.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7282.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7701.c $(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_st7701.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(DependSuffix): ../device/lcd/lcd_rgb_st7701.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_st7701.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7701.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7701.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7701_HSD4.95.c $(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_st7701_HSD4.95.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(DependSuffix): ../device/lcd/lcd_rgb_st7701_HSD4.95.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_st7701_HSD4.95.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7701_HSD4.95.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7701_HSD4.95.c"

$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix): ../device/lcd/lcd_rgb_9503v.c $(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_9503v.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(DependSuffix): ../device/lcd/lcd_rgb_9503v.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_9503v.c"

$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_9503v.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_9503v.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7735S_spimode.c $(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_st7735S_spimode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(DependSuffix): ../device/lcd/lcd_mcu_st7735S_spimode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_st7735S_spimode.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7735S_spimode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7735S_spimode.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7789V_spimode.c $(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_mcu_st7789V_spimode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(DependSuffix): ../device/lcd/lcd_mcu_st7789V_spimode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(DependSuffix) -MM "../device/lcd/lcd_mcu_st7789V_spimode.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7789V_spimode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7789V_spimode.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7701S.c $(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_st7701S.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(DependSuffix): ../device/lcd/lcd_rgb_st7701S.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_st7701S.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7701S.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7701S.c"

$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix): ../device/lcd/lcd_rgb_9503.c $(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/lcd/lcd_rgb_9503.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(DependSuffix): ../device/lcd/lcd_rgb_9503.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(DependSuffix) -MM "../device/lcd/lcd_rgb_9503.c"

$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_9503.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_9503.c"

$(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix): ../device/sensor/COMMON_table.c $(IntermediateDirectory)/sensor_COMMON_table.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/COMMON_table.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_COMMON_table.c$(DependSuffix): ../device/sensor/COMMON_table.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_COMMON_table.c$(DependSuffix) -MM "../device/sensor/COMMON_table.c"

$(IntermediateDirectory)/sensor_COMMON_table.c$(PreprocessSuffix): ../device/sensor/COMMON_table.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_COMMON_table.c$(PreprocessSuffix) "../device/sensor/COMMON_table.c"

$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix): ../device/sensor/SENSOR_BF2013.c $(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_BF2013.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(DependSuffix): ../device/sensor/SENSOR_BF2013.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(DependSuffix) -MM "../device/sensor/SENSOR_BF2013.c"

$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF2013.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF2013.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix): ../device/sensor/SENSOR_BF3016.c $(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_BF3016.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(DependSuffix): ../device/sensor/SENSOR_BF3016.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(DependSuffix) -MM "../device/sensor/SENSOR_BF3016.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF3016.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF3016.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix): ../device/sensor/SENSOR_BF3703.c $(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_BF3703.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(DependSuffix): ../device/sensor/SENSOR_BF3703.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(DependSuffix) -MM "../device/sensor/SENSOR_BF3703.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF3703.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF3703.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix): ../device/sensor/SENSOR_BF3a03.c $(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_BF3a03.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(DependSuffix): ../device/sensor/SENSOR_BF3a03.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(DependSuffix) -MM "../device/sensor/SENSOR_BF3a03.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF3a03.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF3a03.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0307.c $(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC0307.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(DependSuffix): ../device/sensor/SENSOR_GC0307.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC0307.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0307.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0307.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0308.c $(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC0308.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(DependSuffix): ../device/sensor/SENSOR_GC0308.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC0308.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0308.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0308.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0309.c $(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC0309.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(DependSuffix): ../device/sensor/SENSOR_GC0309.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC0309.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0309.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0309.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0328.c $(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC0328.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(DependSuffix): ../device/sensor/SENSOR_GC0328.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC0328.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0328.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0328.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1004.c $(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC1004.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(DependSuffix): ../device/sensor/SENSOR_GC1004.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC1004.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1004.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1004.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1034.c $(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC1034.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(DependSuffix): ../device/sensor/SENSOR_GC1034.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC1034.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1034.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1034.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1054.c $(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC1054.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(DependSuffix): ../device/sensor/SENSOR_GC1054.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC1054.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1054.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1054.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1064.c $(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC1064.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(DependSuffix): ../device/sensor/SENSOR_GC1064.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC1064.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1064.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1064.c"

$(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix): ../device/sensor/SENSOR_H42.c $(IntermediateDirectory)/sensor_SENSOR_H42.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_H42.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H42.c$(DependSuffix): ../device/sensor/SENSOR_H42.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_H42.c$(DependSuffix) -MM "../device/sensor/SENSOR_H42.c"

$(IntermediateDirectory)/sensor_SENSOR_H42.c$(PreprocessSuffix): ../device/sensor/SENSOR_H42.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H42.c$(PreprocessSuffix) "../device/sensor/SENSOR_H42.c"

$(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix): ../device/sensor/SENSOR_H62.c $(IntermediateDirectory)/sensor_SENSOR_H62.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_H62.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H62.c$(DependSuffix): ../device/sensor/SENSOR_H62.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_H62.c$(DependSuffix) -MM "../device/sensor/SENSOR_H62.c"

$(IntermediateDirectory)/sensor_SENSOR_H62.c$(PreprocessSuffix): ../device/sensor/SENSOR_H62.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H62.c$(PreprocessSuffix) "../device/sensor/SENSOR_H62.c"

$(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix): ../device/sensor/SENSOR_H65.c $(IntermediateDirectory)/sensor_SENSOR_H65.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_H65.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H65.c$(DependSuffix): ../device/sensor/SENSOR_H65.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_H65.c$(DependSuffix) -MM "../device/sensor/SENSOR_H65.c"

$(IntermediateDirectory)/sensor_SENSOR_H65.c$(PreprocessSuffix): ../device/sensor/SENSOR_H65.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H65.c$(PreprocessSuffix) "../device/sensor/SENSOR_H65.c"

$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix): ../device/sensor/SENSOR_H7640.c $(IntermediateDirectory)/sensor_SENSOR_H7640.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_H7640.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(DependSuffix): ../device/sensor/SENSOR_H7640.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(DependSuffix) -MM "../device/sensor/SENSOR_H7640.c"

$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(PreprocessSuffix): ../device/sensor/SENSOR_H7640.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H7640.c$(PreprocessSuffix) "../device/sensor/SENSOR_H7640.c"

$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix): ../device/sensor/SENSOR_HM1055.c $(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_HM1055.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(DependSuffix): ../device/sensor/SENSOR_HM1055.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(DependSuffix) -MM "../device/sensor/SENSOR_HM1055.c"

$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(PreprocessSuffix): ../device/sensor/SENSOR_HM1055.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(PreprocessSuffix) "../device/sensor/SENSOR_HM1055.c"

$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix): ../device/sensor/SENSOR_IT03A1.c $(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_IT03A1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(DependSuffix): ../device/sensor/SENSOR_IT03A1.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(DependSuffix) -MM "../device/sensor/SENSOR_IT03A1.c"

$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(PreprocessSuffix): ../device/sensor/SENSOR_IT03A1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(PreprocessSuffix) "../device/sensor/SENSOR_IT03A1.c"

$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix): ../device/sensor/SENSOR_NT99141.c $(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_NT99141.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(DependSuffix): ../device/sensor/SENSOR_NT99141.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(DependSuffix) -MM "../device/sensor/SENSOR_NT99141.c"

$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(PreprocessSuffix): ../device/sensor/SENSOR_NT99141.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(PreprocessSuffix) "../device/sensor/SENSOR_NT99141.c"

$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix): ../device/sensor/SENSOR_NT99142.c $(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_NT99142.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(DependSuffix): ../device/sensor/SENSOR_NT99142.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(DependSuffix) -MM "../device/sensor/SENSOR_NT99142.c"

$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(PreprocessSuffix): ../device/sensor/SENSOR_NT99142.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(PreprocessSuffix) "../device/sensor/SENSOR_NT99142.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix): ../device/sensor/SENSOR_OV7670.c $(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_OV7670.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(DependSuffix): ../device/sensor/SENSOR_OV7670.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(DependSuffix) -MM "../device/sensor/SENSOR_OV7670.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV7670.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV7670.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix): ../device/sensor/SENSOR_OV7725.c $(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_OV7725.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(DependSuffix): ../device/sensor/SENSOR_OV7725.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(DependSuffix) -MM "../device/sensor/SENSOR_OV7725.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV7725.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV7725.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix): ../device/sensor/SENSOR_OV7736.c $(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_OV7736.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(DependSuffix): ../device/sensor/SENSOR_OV7736.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(DependSuffix) -MM "../device/sensor/SENSOR_OV7736.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV7736.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV7736.c"

$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix): ../device/sensor/SENSOR_OV9710.c $(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_OV9710.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(DependSuffix): ../device/sensor/SENSOR_OV9710.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(DependSuffix) -MM "../device/sensor/SENSOR_OV9710.c"

$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV9710.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV9710.c"

$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix): ../device/sensor/SENSOR_OV9732.c $(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_OV9732.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(DependSuffix): ../device/sensor/SENSOR_OV9732.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(DependSuffix) -MM "../device/sensor/SENSOR_OV9732.c"

$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV9732.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV9732.c"

$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix): ../device/sensor/SENSOR_SC1045.c $(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SC1045.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(DependSuffix): ../device/sensor/SENSOR_SC1045.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(DependSuffix) -MM "../device/sensor/SENSOR_SC1045.c"

$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(PreprocessSuffix): ../device/sensor/SENSOR_SC1045.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(PreprocessSuffix) "../device/sensor/SENSOR_SC1045.c"

$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix): ../device/sensor/SENSOR_SC1243.c $(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SC1243.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(DependSuffix): ../device/sensor/SENSOR_SC1243.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(DependSuffix) -MM "../device/sensor/SENSOR_SC1243.c"

$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(PreprocessSuffix): ../device/sensor/SENSOR_SC1243.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(PreprocessSuffix) "../device/sensor/SENSOR_SC1243.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix): ../device/sensor/SENSOR_SIV100B.c $(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SIV100B.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(DependSuffix): ../device/sensor/SENSOR_SIV100B.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(DependSuffix) -MM "../device/sensor/SENSOR_SIV100B.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(PreprocessSuffix): ../device/sensor/SENSOR_SIV100B.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(PreprocessSuffix) "../device/sensor/SENSOR_SIV100B.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix): ../device/sensor/SENSOR_SIV120B.c $(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SIV120B.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(DependSuffix): ../device/sensor/SENSOR_SIV120B.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(DependSuffix) -MM "../device/sensor/SENSOR_SIV120B.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(PreprocessSuffix): ../device/sensor/SENSOR_SIV120B.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(PreprocessSuffix) "../device/sensor/SENSOR_SIV120B.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix): ../device/sensor/SENSOR_SIV121DS.c $(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SIV121DS.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(DependSuffix): ../device/sensor/SENSOR_SIV121DS.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(DependSuffix) -MM "../device/sensor/SENSOR_SIV121DS.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(PreprocessSuffix): ../device/sensor/SENSOR_SIV121DS.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(PreprocessSuffix) "../device/sensor/SENSOR_SIV121DS.c"

$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix): ../device/sensor/SENSOR_SP1409.c $(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SP1409.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(DependSuffix): ../device/sensor/SENSOR_SP1409.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(DependSuffix) -MM "../device/sensor/SENSOR_SP1409.c"

$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(PreprocessSuffix): ../device/sensor/SENSOR_SP1409.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(PreprocessSuffix) "../device/sensor/SENSOR_SP1409.c"

$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix): ../device/sensor/SENSOR_SP140A.c $(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SP140A.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(DependSuffix): ../device/sensor/SENSOR_SP140A.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(DependSuffix) -MM "../device/sensor/SENSOR_SP140A.c"

$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(PreprocessSuffix): ../device/sensor/SENSOR_SP140A.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(PreprocessSuffix) "../device/sensor/SENSOR_SP140A.c"

$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix): ../device/sensor/SNESOR_FPX1002.c $(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SNESOR_FPX1002.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(DependSuffix): ../device/sensor/SNESOR_FPX1002.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(DependSuffix) -MM "../device/sensor/SNESOR_FPX1002.c"

$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(PreprocessSuffix): ../device/sensor/SNESOR_FPX1002.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(PreprocessSuffix) "../device/sensor/SNESOR_FPX1002.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1084.c $(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_GC1084.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(DependSuffix): ../device/sensor/SENSOR_GC1084.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(DependSuffix) -MM "../device/sensor/SENSOR_GC1084.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1084.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1084.c"

$(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix): ../device/sensor/SENSOR_H63.c $(IntermediateDirectory)/sensor_SENSOR_H63.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_H63.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H63.c$(DependSuffix): ../device/sensor/SENSOR_H63.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_H63.c$(DependSuffix) -MM "../device/sensor/SENSOR_H63.c"

$(IntermediateDirectory)/sensor_SENSOR_H63.c$(PreprocessSuffix): ../device/sensor/SENSOR_H63.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H63.c$(PreprocessSuffix) "../device/sensor/SENSOR_H63.c"

$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix): ../device/sensor/SENSOR_SC2331.c $(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/sensor/SENSOR_SC2331.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(DependSuffix): ../device/sensor/SENSOR_SC2331.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(DependSuffix) -MM "../device/sensor/SENSOR_SC2331.c"

$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(PreprocessSuffix): ../device/sensor/SENSOR_SC2331.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(PreprocessSuffix) "../device/sensor/SENSOR_SC2331.c"

$(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix): ../device/tp/icn85xx.c $(IntermediateDirectory)/tp_icn85xx.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/tp/icn85xx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_icn85xx.c$(DependSuffix): ../device/tp/icn85xx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tp_icn85xx.c$(DependSuffix) -MM "../device/tp/icn85xx.c"

$(IntermediateDirectory)/tp_icn85xx.c$(PreprocessSuffix): ../device/tp/icn85xx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_icn85xx.c$(PreprocessSuffix) "../device/tp/icn85xx.c"

$(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix): ../device/tp/icn85xx_flash.c $(IntermediateDirectory)/tp_icn85xx_flash.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/tp/icn85xx_flash.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_icn85xx_flash.c$(DependSuffix): ../device/tp/icn85xx_flash.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tp_icn85xx_flash.c$(DependSuffix) -MM "../device/tp/icn85xx_flash.c"

$(IntermediateDirectory)/tp_icn85xx_flash.c$(PreprocessSuffix): ../device/tp/icn85xx_flash.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_icn85xx_flash.c$(PreprocessSuffix) "../device/tp/icn85xx_flash.c"

$(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix): ../device/tp/icnt81.c $(IntermediateDirectory)/tp_icnt81.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/tp/icnt81.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_icnt81.c$(DependSuffix): ../device/tp/icnt81.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tp_icnt81.c$(DependSuffix) -MM "../device/tp/icnt81.c"

$(IntermediateDirectory)/tp_icnt81.c$(PreprocessSuffix): ../device/tp/icnt81.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_icnt81.c$(PreprocessSuffix) "../device/tp/icnt81.c"

$(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix): ../device/tp/touchPanel.c $(IntermediateDirectory)/tp_touchPanel.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/tp/touchPanel.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_touchPanel.c$(DependSuffix): ../device/tp/touchPanel.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tp_touchPanel.c$(DependSuffix) -MM "../device/tp/touchPanel.c"

$(IntermediateDirectory)/tp_touchPanel.c$(PreprocessSuffix): ../device/tp/touchPanel.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_touchPanel.c$(PreprocessSuffix) "../device/tp/touchPanel.c"

$(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix): ../device/tp/tpiic.c $(IntermediateDirectory)/tp_tpiic.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/device/tp/tpiic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_tpiic.c$(DependSuffix): ../device/tp/tpiic.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tp_tpiic.c$(DependSuffix) -MM "../device/tp/tpiic.c"

$(IntermediateDirectory)/tp_tpiic.c$(PreprocessSuffix): ../device/tp/tpiic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_tpiic.c$(PreprocessSuffix) "../device/tp/tpiic.c"

$(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix): ../../board/AX3292/board.c $(IntermediateDirectory)/AX3292_board.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/board/AX3292/board.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_board.c$(DependSuffix): ../../board/AX3292/board.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix) -MF$(IntermediateDirectory)/AX3292_board.c$(DependSuffix) -MM "../../board/AX3292/board.c"

$(IntermediateDirectory)/AX3292_board.c$(PreprocessSuffix): ../../board/AX3292/board.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_board.c$(PreprocessSuffix) "../../board/AX3292/board.c"

$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix): ../../board/AX3292/build-nd.s $(IntermediateDirectory)/AX3292_build-nd.s$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/board/AX3292/build-nd.s" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_build-nd.s$(DependSuffix): ../../board/AX3292/build-nd.s
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) -MF$(IntermediateDirectory)/AX3292_build-nd.s$(DependSuffix) -MM "../../board/AX3292/build-nd.s"

$(IntermediateDirectory)/AX3292_build-nd.s$(PreprocessSuffix): ../../board/AX3292/build-nd.s
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_build-nd.s$(PreprocessSuffix) "../../board/AX3292/build-nd.s"

$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix): ../../board/AX3292/spi_boot_cfg.S $(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/board/AX3292/spi_boot_cfg.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(DependSuffix): ../../board/AX3292/spi_boot_cfg.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix) -MF$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(DependSuffix) -MM "../../board/AX3292/spi_boot_cfg.S"

$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(PreprocessSuffix): ../../board/AX3292/spi_boot_cfg.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(PreprocessSuffix) "../../board/AX3292/spi_boot_cfg.S"

$(IntermediateDirectory)/game_game_block.c$(ObjectSuffix): task/game/game_block.c $(IntermediateDirectory)/game_game_block.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/game_block.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_block.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_block.c$(DependSuffix): task/game/game_block.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_game_block.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_game_block.c$(DependSuffix) -MM "task/game/game_block.c"

$(IntermediateDirectory)/game_game_block.c$(PreprocessSuffix): task/game/game_block.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_block.c$(PreprocessSuffix) "task/game/game_block.c"

$(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix): task/game/game_maze.c $(IntermediateDirectory)/game_game_maze.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/game_maze.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_maze.c$(DependSuffix): task/game/game_maze.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_game_maze.c$(DependSuffix) -MM "task/game/game_maze.c"

$(IntermediateDirectory)/game_game_maze.c$(PreprocessSuffix): task/game/game_maze.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_maze.c$(PreprocessSuffix) "task/game/game_maze.c"

$(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix): task/game/game_snake.c $(IntermediateDirectory)/game_game_snake.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/game_snake.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_snake.c$(DependSuffix): task/game/game_snake.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_game_snake.c$(DependSuffix) -MM "task/game/game_snake.c"

$(IntermediateDirectory)/game_game_snake.c$(PreprocessSuffix): task/game/game_snake.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_snake.c$(PreprocessSuffix) "task/game/game_snake.c"

$(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix): task/game/game_sokoban.c $(IntermediateDirectory)/game_game_sokoban.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/game_sokoban.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_sokoban.c$(DependSuffix): task/game/game_sokoban.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_game_sokoban.c$(DependSuffix) -MM "task/game/game_sokoban.c"

$(IntermediateDirectory)/game_game_sokoban.c$(PreprocessSuffix): task/game/game_sokoban.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_sokoban.c$(PreprocessSuffix) "task/game/game_sokoban.c"

$(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix): task/game/game_tetris.c $(IntermediateDirectory)/game_game_tetris.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/game_tetris.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_tetris.c$(DependSuffix): task/game/game_tetris.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_game_tetris.c$(DependSuffix) -MM "task/game/game_tetris.c"

$(IntermediateDirectory)/game_game_tetris.c$(PreprocessSuffix): task/game/game_tetris.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_tetris.c$(PreprocessSuffix) "task/game/game_tetris.c"

$(IntermediateDirectory)/game_taskGame.c$(ObjectSuffix): task/game/taskGame.c $(IntermediateDirectory)/game_taskGame.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/taskGame.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_taskGame.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_taskGame.c$(DependSuffix): task/game/taskGame.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_taskGame.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_taskGame.c$(DependSuffix) -MM "task/game/taskGame.c"

$(IntermediateDirectory)/game_taskGame.c$(PreprocessSuffix): task/game/taskGame.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_taskGame.c$(PreprocessSuffix) "task/game/taskGame.c"

$(IntermediateDirectory)/game_taskGameMenu.c$(ObjectSuffix): task/game/taskGameMenu.c $(IntermediateDirectory)/game_taskGameMenu.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/taskGameMenu.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_taskGameMenu.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_taskGameMenu.c$(DependSuffix): task/game/taskGameMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_taskGameMenu.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_taskGameMenu.c$(DependSuffix) -MM "task/game/taskGameMenu.c"

$(IntermediateDirectory)/game_taskGameMenu.c$(PreprocessSuffix): task/game/taskGameMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_taskGameMenu.c$(PreprocessSuffix) "task/game/taskGameMenu.c"

$(IntermediateDirectory)/game_taskGameMenuMsg.c$(ObjectSuffix): task/game/taskGameMenuMsg.c $(IntermediateDirectory)/game_taskGameMenuMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/taskGameMenuMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_taskGameMenuMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_taskGameMenuMsg.c$(DependSuffix): task/game/taskGameMenuMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_taskGameMenuMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_taskGameMenuMsg.c$(DependSuffix) -MM "task/game/taskGameMenuMsg.c"

$(IntermediateDirectory)/game_taskGameMenuMsg.c$(PreprocessSuffix): task/game/taskGameMenuMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_taskGameMenuMsg.c$(PreprocessSuffix) "task/game/taskGameMenuMsg.c"

$(IntermediateDirectory)/game_taskGameMenuRes.c$(ObjectSuffix): task/game/taskGameMenuRes.c $(IntermediateDirectory)/game_taskGameMenuRes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/taskGameMenuRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_taskGameMenuRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_taskGameMenuRes.c$(DependSuffix): task/game/taskGameMenuRes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_taskGameMenuRes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_taskGameMenuRes.c$(DependSuffix) -MM "task/game/taskGameMenuRes.c"

$(IntermediateDirectory)/game_taskGameMenuRes.c$(PreprocessSuffix): task/game/taskGameMenuRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_taskGameMenuRes.c$(PreprocessSuffix) "task/game/taskGameMenuRes.c"

$(IntermediateDirectory)/game_taskGameMsg.c$(ObjectSuffix): task/game/taskGameMsg.c $(IntermediateDirectory)/game_taskGameMsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/taskGameMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_taskGameMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_taskGameMsg.c$(DependSuffix): task/game/taskGameMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_taskGameMsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_taskGameMsg.c$(DependSuffix) -MM "task/game/taskGameMsg.c"

$(IntermediateDirectory)/game_taskGameMsg.c$(PreprocessSuffix): task/game/taskGameMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_taskGameMsg.c$(PreprocessSuffix) "task/game/taskGameMsg.c"

$(IntermediateDirectory)/xos_xmsgq.c$(ObjectSuffix): task/game/xos/xmsgq.c $(IntermediateDirectory)/xos_xmsgq.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/xos/xmsgq.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/xos_xmsgq.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/xos_xmsgq.c$(DependSuffix): task/game/xos/xmsgq.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/xos_xmsgq.c$(ObjectSuffix) -MF$(IntermediateDirectory)/xos_xmsgq.c$(DependSuffix) -MM "task/game/xos/xmsgq.c"

$(IntermediateDirectory)/xos_xmsgq.c$(PreprocessSuffix): task/game/xos/xmsgq.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/xos_xmsgq.c$(PreprocessSuffix) "task/game/xos/xmsgq.c"

$(IntermediateDirectory)/game_display.c$(ObjectSuffix): task/game/display.c $(IntermediateDirectory)/game_display.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/display.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_display.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_display.c$(DependSuffix): task/game/display.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/game_display.c$(ObjectSuffix) -MF$(IntermediateDirectory)/game_display.c$(DependSuffix) -MM "task/game/display.c"

$(IntermediateDirectory)/game_display.c$(PreprocessSuffix): task/game/display.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_display.c$(PreprocessSuffix) "task/game/display.c"

$(IntermediateDirectory)/xos_xos.c$(ObjectSuffix): task/game/xos/xos.c $(IntermediateDirectory)/xos_xos.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/xos/xos.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/xos_xos.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/xos_xos.c$(DependSuffix): task/game/xos/xos.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/xos_xos.c$(ObjectSuffix) -MF$(IntermediateDirectory)/xos_xos.c$(DependSuffix) -MM "task/game/xos/xos.c"

$(IntermediateDirectory)/xos_xos.c$(PreprocessSuffix): task/game/xos/xos.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/xos_xos.c$(PreprocessSuffix) "task/game/xos/xos.c"

$(IntermediateDirectory)/xos_x_cpu.c$(ObjectSuffix): task/game/xos/x_cpu.c $(IntermediateDirectory)/xos_x_cpu.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/xos/x_cpu.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/xos_x_cpu.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/xos_x_cpu.c$(DependSuffix): task/game/xos/x_cpu.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/xos_x_cpu.c$(ObjectSuffix) -MF$(IntermediateDirectory)/xos_x_cpu.c$(DependSuffix) -MM "task/game/xos/x_cpu.c"

$(IntermediateDirectory)/xos_x_cpu.c$(PreprocessSuffix): task/game/xos/x_cpu.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/xos_x_cpu.c$(PreprocessSuffix) "task/game/xos/x_cpu.c"

$(IntermediateDirectory)/xos_xmbox.c$(ObjectSuffix): task/game/xos/xmbox.c $(IntermediateDirectory)/xos_xmbox.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/xos/xmbox.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/xos_xmbox.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/xos_xmbox.c$(DependSuffix): task/game/xos/xmbox.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/xos_xmbox.c$(ObjectSuffix) -MF$(IntermediateDirectory)/xos_xmbox.c$(DependSuffix) -MM "task/game/xos/xmbox.c"

$(IntermediateDirectory)/xos_xmbox.c$(PreprocessSuffix): task/game/xos/xmbox.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/xos_xmbox.c$(PreprocessSuffix) "task/game/xos/xmbox.c"

$(IntermediateDirectory)/xos_xwork.c$(ObjectSuffix): task/game/xos/xwork.c $(IntermediateDirectory)/xos_xwork.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/demo/helloftv/task/game/xos/xwork.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/xos_xwork.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/xos_xwork.c$(DependSuffix): task/game/xos/xwork.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/xos_xwork.c$(ObjectSuffix) -MF$(IntermediateDirectory)/xos_xwork.c$(DependSuffix) -MM "task/game/xos/xwork.c"

$(IntermediateDirectory)/xos_xwork.c$(PreprocessSuffix): task/game/xos/xwork.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/xos_xwork.c$(PreprocessSuffix) "task/game/xos/xwork.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


