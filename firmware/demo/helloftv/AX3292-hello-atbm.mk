##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-hello-atbm
ConfigurationName      :=Debug
WorkspacePath          := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\demo\helloftv"
ProjectPath            := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\demo\helloftv"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=TheBlob
Date                   :=03/09/2025
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=C:/MinGW-4.7.1/bin/or1k-elf-ld.exe
SharedObjectLinkerName :=C:/MinGW-4.7.1/bin/or1k-elf-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=
PreprocessSuffix       :=.i
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
ObjectsFileList        :="AX3292-hello-atbm.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  -T../../board/AX3292/mvl8801/AX3292.ld -Map=$(IntermediateDirectory)/$(ProjectName).map --gc-sections -( -l_atbm -lAiCameraLicenseAtbm -lAX3292-sys -lAX3292-lib20 -lAX3292-libex -lAX3292-os -lAX3292-lwip -lAX3292-mcu -lc -lm -lgcc -lnosys -)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../ $(IncludeSwitch)../../inc $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../board/AX3292/mvl8801 $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/api/inc $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../sys $(IncludeSwitch)../../lib $(IncludeSwitch)../../lib/drv $(IncludeSwitch)../../lib/mediaFile/mp3/inc $(IncludeSwitch)../../net $(IncludeSwitch)./../../net/atbm/api $(IncludeSwitch)./../../net/atbm/include $(IncludeSwitch)./../../net/atbm/hal/include $(IncludeSwitch)./../../net/atbm/hal/sdio $(IncludeSwitch)./../../net/atbm/api/sdio $(IncludeSwitch)./../../net/atbm/net/include $(IncludeSwitch)./../../net/atbm/net/include/proto $(IncludeSwitch)./../../net/atbm/net/wpa/crypto/include $(IncludeSwitch)./../../net/atbm/net/wpa/wps/include $(IncludeSwitch)./../../net/atbm/net/wpa/sae/include $(IncludeSwitch)./../../net/atbm/os/jrx_rtos/include $(IncludeSwitch)./../../net/atbm/os/include $(IncludeSwitch)../../sys/drv $(IncludeSwitch)../../sys/iperf2 $(IncludeSwitch)../../sys/drv/sensor/ $(IncludeSwitch)../../sys/finsh/ $(IncludeSwitch)../../inc/net $(IncludeSwitch)../../arch/AX3292/hal/inc $(IncludeSwitch)../../arch/AX3292/hal/ $(IncludeSwitch)../../arch/AX3292/gpio_ops/ $(IncludeSwitch)../../arch/AX3292/ax32xx/inc $(IncludeSwitch)../../arch/AX3292/inc/asm $(IncludeSwitch)../device/tp 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)C:/MinGW-4.7.1/lib/gcc/or1k-elf/4.9.1/no-delay $(LibraryPathSwitch)C:/MinGW-4.7.1/or1k-elf/lib/no-delay  $(LibraryPathSwitch). $(LibraryPathSwitch)../../License $(LibraryPathSwitch)../../sdk $(LibraryPathSwitch)../../sdk/thirdparty 

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
	$(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu2.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu2Msg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu2Res.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu3.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu3Msg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funMainMenu3Res.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcljfNetTypeMenu.c$(ObjectSuffix) $(IntermediateDirectory)/task_funcljfNetTypeMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_wifiProc.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_iperf2_adapt.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiPhotodetailMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiPhotodetailRes.c$(ObjectSuffix) $(IntermediateDirectory)/task_aiAudioDownload.c$(ObjectSuffix) $(IntermediateDirectory)/task_funRoleSwitch.c$(ObjectSuffix) $(IntermediateDirectory)/task_funRoleSwitchMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funRoleSwitchRes.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiSmallModelMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiSmallModelRes.c$(ObjectSuffix) $(IntermediateDirectory)/task_uiMenuVersionRes.c$(ObjectSuffix) \
	$(IntermediateDirectory)/task_funApp.c$(ObjectSuffix) $(IntermediateDirectory)/task_funAppMsg.c$(ObjectSuffix) $(IntermediateDirectory)/task_funAppRes.c$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/task_uiMenuVersionRes.c$(ObjectSuffix) $(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix) $(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix) \
	$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix) \
	$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcd_mcu_nv3030b.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_BF20A6.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_SENSOR_H63P.c$(ObjectSuffix) $(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix) \
	$(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix) $(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix) $(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix) $(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix) $(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix) $(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) $(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix) $(IntermediateDirectory)/game_funcGame.c$(ObjectSuffix) $(IntermediateDirectory)/game_funcGameMenu.c$(ObjectSuffix) $(IntermediateDirectory)/game_funcGameMenuMsg.c$(ObjectSuffix) \
	$(IntermediateDirectory)/game_funcGameMsg.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_block.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix) $(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix) 



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
	..\..\output\boottool AX3292-Config-atbm.dat
	..\..\output\make.bat
	@echo Done

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/task_uiWin.c$(ObjectSuffix): task/uiWin.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiWin.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiWin.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiWin.c$(PreprocessSuffix): task/uiWin.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiWin.c$(PreprocessSuffix) "task/uiWin.c"

$(IntermediateDirectory)/task_msgDeal.c$(ObjectSuffix): task/msgDeal.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/msgDeal.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_msgDeal.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_msgDeal.c$(PreprocessSuffix): task/msgDeal.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_msgDeal.c$(PreprocessSuffix) "task/msgDeal.c"

$(IntermediateDirectory)/task_msgSysRecommend.c$(ObjectSuffix): task/msgSysRecommend.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/msgSysRecommend.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_msgSysRecommend.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_msgSysRecommend.c$(PreprocessSuffix): task/msgSysRecommend.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_msgSysRecommend.c$(PreprocessSuffix) "task/msgSysRecommend.c"

$(IntermediateDirectory)/task_msgSysUnshielded.c$(ObjectSuffix): task/msgSysUnshielded.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/msgSysUnshielded.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_msgSysUnshielded.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_msgSysUnshielded.c$(PreprocessSuffix): task/msgSysUnshielded.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_msgSysUnshielded.c$(PreprocessSuffix) "task/msgSysUnshielded.c"

$(IntermediateDirectory)/task_taskUI.c$(ObjectSuffix): task/taskUI.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/taskUI.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_taskUI.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_taskUI.c$(PreprocessSuffix): task/taskUI.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_taskUI.c$(PreprocessSuffix) "task/taskUI.c"

$(IntermediateDirectory)/task_uiTipsMsg.c$(ObjectSuffix): task/uiTipsMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiTipsMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiTipsMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiTipsMsg.c$(PreprocessSuffix): task/uiTipsMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiTipsMsg.c$(PreprocessSuffix) "task/uiTipsMsg.c"

$(IntermediateDirectory)/task_taskScan.c$(ObjectSuffix): task/taskScan.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/taskScan.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_taskScan.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_taskScan.c$(PreprocessSuffix): task/taskScan.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_taskScan.c$(PreprocessSuffix) "task/taskScan.c"

$(IntermediateDirectory)/task_funcAudioPlayer.c$(ObjectSuffix): task/funcAudioPlayer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcAudioPlayer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioPlayer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioPlayer.c$(PreprocessSuffix): task/funcAudioPlayer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioPlayer.c$(PreprocessSuffix) "task/funcAudioPlayer.c"

$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(ObjectSuffix): task/funcAudioPlayerMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcAudioPlayerMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(PreprocessSuffix): task/funcAudioPlayerMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioPlayerMsg.c$(PreprocessSuffix) "task/funcAudioPlayerMsg.c"

$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(ObjectSuffix): task/funcAudioPlayerRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcAudioPlayerRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioPlayerRes.c$(PreprocessSuffix): task/funcAudioPlayerRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioPlayerRes.c$(PreprocessSuffix) "task/funcAudioPlayerRes.c"

$(IntermediateDirectory)/task_funcVideoRecord.c$(ObjectSuffix): task/funcVideoRecord.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcVideoRecord.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcVideoRecord.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcVideoRecord.c$(PreprocessSuffix): task/funcVideoRecord.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcVideoRecord.c$(PreprocessSuffix) "task/funcVideoRecord.c"

$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(ObjectSuffix): task/funcVideoRecordMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcVideoRecordMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcVideoRecordMsg.c$(PreprocessSuffix): task/funcVideoRecordMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcVideoRecordMsg.c$(PreprocessSuffix) "task/funcVideoRecordMsg.c"

$(IntermediateDirectory)/task_funcPlayBack.c$(ObjectSuffix): task/funcPlayBack.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcPlayBack.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPlayBack.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPlayBack.c$(PreprocessSuffix): task/funcPlayBack.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPlayBack.c$(PreprocessSuffix) "task/funcPlayBack.c"

$(IntermediateDirectory)/task_funcPlayBackMsg.c$(ObjectSuffix): task/funcPlayBackMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcPlayBackMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPlayBackMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPlayBackMsg.c$(PreprocessSuffix): task/funcPlayBackMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPlayBackMsg.c$(PreprocessSuffix) "task/funcPlayBackMsg.c"

$(IntermediateDirectory)/task_funcWifi.c$(ObjectSuffix): task/funcWifi.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcWifi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcWifi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcWifi.c$(PreprocessSuffix): task/funcWifi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcWifi.c$(PreprocessSuffix) "task/funcWifi.c"

$(IntermediateDirectory)/task_funcWifiMsg.c$(ObjectSuffix): task/funcWifiMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcWifiMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcWifiMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcWifiMsg.c$(PreprocessSuffix): task/funcWifiMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcWifiMsg.c$(PreprocessSuffix) "task/funcWifiMsg.c"

$(IntermediateDirectory)/task_drvKey.c$(ObjectSuffix): task/drvKey.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/drvKey.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_drvKey.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_drvKey.c$(PreprocessSuffix): task/drvKey.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_drvKey.c$(PreprocessSuffix) "task/drvKey.c"

$(IntermediateDirectory)/task_drvBattery.c$(ObjectSuffix): task/drvBattery.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/drvBattery.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_drvBattery.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_drvBattery.c$(PreprocessSuffix): task/drvBattery.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_drvBattery.c$(PreprocessSuffix) "task/drvBattery.c"

$(IntermediateDirectory)/task_font.c$(ObjectSuffix): task/font.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/font.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_font.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_font.c$(PreprocessSuffix): task/font.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_font.c$(PreprocessSuffix) "task/font.c"

$(IntermediateDirectory)/task_icon.c$(ObjectSuffix): task/icon.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/icon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_icon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_icon.c$(PreprocessSuffix): task/icon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_icon.c$(PreprocessSuffix) "task/icon.c"

$(IntermediateDirectory)/task_config.c$(ObjectSuffix): task/config.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/config.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_config.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_config.c$(PreprocessSuffix): task/config.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_config.c$(PreprocessSuffix) "task/config.c"

$(IntermediateDirectory)/task_configGetValue.c$(ObjectSuffix): task/configGetValue.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/configGetValue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_configGetValue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_configGetValue.c$(PreprocessSuffix): task/configGetValue.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_configGetValue.c$(PreprocessSuffix) "task/configGetValue.c"

$(IntermediateDirectory)/task_configSystem.c$(ObjectSuffix): task/configSystem.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/configSystem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_configSystem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_configSystem.c$(PreprocessSuffix): task/configSystem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_configSystem.c$(PreprocessSuffix) "task/configSystem.c"

$(IntermediateDirectory)/task_menuMovieRec.c$(ObjectSuffix): task/menuMovieRec.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/menuMovieRec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_menuMovieRec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_menuMovieRec.c$(PreprocessSuffix): task/menuMovieRec.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_menuMovieRec.c$(PreprocessSuffix) "task/menuMovieRec.c"

$(IntermediateDirectory)/task_menuPlayBack.c$(ObjectSuffix): task/menuPlayBack.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/menuPlayBack.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_menuPlayBack.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_menuPlayBack.c$(PreprocessSuffix): task/menuPlayBack.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_menuPlayBack.c$(PreprocessSuffix) "task/menuPlayBack.c"

$(IntermediateDirectory)/task_toolFunc.c$(ObjectSuffix): task/toolFunc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/toolFunc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_toolFunc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_toolFunc.c$(PreprocessSuffix): task/toolFunc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_toolFunc.c$(PreprocessSuffix) "task/toolFunc.c"

$(IntermediateDirectory)/task_uiAsternMsg.c$(ObjectSuffix): task/uiAsternMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiAsternMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiAsternMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiAsternMsg.c$(PreprocessSuffix): task/uiAsternMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiAsternMsg.c$(PreprocessSuffix) "task/uiAsternMsg.c"

$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(ObjectSuffix): task/uiMenuDateTime1Msg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuDateTime1Msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(PreprocessSuffix): task/uiMenuDateTime1Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDateTime1Msg.c$(PreprocessSuffix) "task/uiMenuDateTime1Msg.c"

$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(ObjectSuffix): task/uiMenuDefaultMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuDefaultMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(PreprocessSuffix): task/uiMenuDefaultMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDefaultMsg.c$(PreprocessSuffix) "task/uiMenuDefaultMsg.c"

$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(ObjectSuffix): task/uiMenuDelAllMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuDelAllMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(PreprocessSuffix): task/uiMenuDelAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDelAllMsg.c$(PreprocessSuffix) "task/uiMenuDelAllMsg.c"

$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(ObjectSuffix): task/uiMenuDelCurMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuDelCurMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(PreprocessSuffix): task/uiMenuDelCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuDelCurMsg.c$(PreprocessSuffix) "task/uiMenuDelCurMsg.c"

$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(ObjectSuffix): task/uiMenuFormatMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuFormatMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuFormatMsg.c$(PreprocessSuffix): task/uiMenuFormatMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuFormatMsg.c$(PreprocessSuffix) "task/uiMenuFormatMsg.c"

$(IntermediateDirectory)/task_uiMenuItemMsg.c$(ObjectSuffix): task/uiMenuItemMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuItemMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuItemMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuItemMsg.c$(PreprocessSuffix): task/uiMenuItemMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuItemMsg.c$(PreprocessSuffix) "task/uiMenuItemMsg.c"

$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(ObjectSuffix): task/uiMenuLockCurMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuLockCurMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(PreprocessSuffix): task/uiMenuLockCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuLockCurMsg.c$(PreprocessSuffix) "task/uiMenuLockCurMsg.c"

$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(ObjectSuffix): task/uiMenuOptionMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuOptionMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuOptionMsg.c$(PreprocessSuffix): task/uiMenuOptionMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuOptionMsg.c$(PreprocessSuffix) "task/uiMenuOptionMsg.c"

$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(ObjectSuffix): task/uiMenuUnlockAllMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuUnlockAllMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(PreprocessSuffix): task/uiMenuUnlockAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuUnlockAllMsg.c$(PreprocessSuffix) "task/uiMenuUnlockAllMsg.c"

$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(ObjectSuffix): task/uiMenuUnlockCurMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuUnlockCurMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(PreprocessSuffix): task/uiMenuUnlockCurMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuUnlockCurMsg.c$(PreprocessSuffix) "task/uiMenuUnlockCurMsg.c"

$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(ObjectSuffix): task/uiMenuUsbMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuUsbMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuUsbMsg.c$(PreprocessSuffix): task/uiMenuUsbMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuUsbMsg.c$(PreprocessSuffix) "task/uiMenuUsbMsg.c"

$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(ObjectSuffix): task/uiMenuVersionMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuVersionMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuVersionMsg.c$(PreprocessSuffix): task/uiMenuVersionMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuVersionMsg.c$(PreprocessSuffix) "task/uiMenuVersionMsg.c"

$(IntermediateDirectory)/task_uiNoFileMsg.c$(ObjectSuffix): task/uiNoFileMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiNoFileMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiNoFileMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiNoFileMsg.c$(PreprocessSuffix): task/uiNoFileMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiNoFileMsg.c$(PreprocessSuffix) "task/uiNoFileMsg.c"

$(IntermediateDirectory)/task_uiSelfTestMsg.c$(ObjectSuffix): task/uiSelfTestMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiSelfTestMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiSelfTestMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiSelfTestMsg.c$(PreprocessSuffix): task/uiSelfTestMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiSelfTestMsg.c$(PreprocessSuffix) "task/uiSelfTestMsg.c"

$(IntermediateDirectory)/task_uiSlideShowMsg.c$(ObjectSuffix): task/uiSlideShowMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiSlideShowMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiSlideShowMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiSlideShowMsg.c$(PreprocessSuffix): task/uiSlideShowMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiSlideShowMsg.c$(PreprocessSuffix) "task/uiSlideShowMsg.c"

$(IntermediateDirectory)/task_uiThumbnallMsg.c$(ObjectSuffix): task/uiThumbnallMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiThumbnallMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiThumbnallMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiThumbnallMsg.c$(PreprocessSuffix): task/uiThumbnallMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiThumbnallMsg.c$(PreprocessSuffix) "task/uiThumbnallMsg.c"

$(IntermediateDirectory)/task_uiTips1Msg.c$(ObjectSuffix): task/uiTips1Msg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiTips1Msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiTips1Msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiTips1Msg.c$(PreprocessSuffix): task/uiTips1Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiTips1Msg.c$(PreprocessSuffix) "task/uiTips1Msg.c"

$(IntermediateDirectory)/task_upgrade.c$(ObjectSuffix): task/upgrade.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/upgrade.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_upgrade.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_upgrade.c$(PreprocessSuffix): task/upgrade.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_upgrade.c$(PreprocessSuffix) "task/upgrade.c"

$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(ObjectSuffix): task/uiMenuLockAllMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuLockAllMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(PreprocessSuffix): task/uiMenuLockAllMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuLockAllMsg.c$(PreprocessSuffix) "task/uiMenuLockAllMsg.c"

$(IntermediateDirectory)/task_funcPowerOff.c$(ObjectSuffix): task/funcPowerOff.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcPowerOff.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPowerOff.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPowerOff.c$(PreprocessSuffix): task/funcPowerOff.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPowerOff.c$(PreprocessSuffix) "task/funcPowerOff.c"

$(IntermediateDirectory)/task_funcUSBDev.c$(ObjectSuffix): task/funcUSBDev.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcUSBDev.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcUSBDev.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcUSBDev.c$(PreprocessSuffix): task/funcUSBDev.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcUSBDev.c$(PreprocessSuffix) "task/funcUSBDev.c"

$(IntermediateDirectory)/task_drvMisc.c$(ObjectSuffix): task/drvMisc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/drvMisc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_drvMisc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_drvMisc.c$(PreprocessSuffix): task/drvMisc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_drvMisc.c$(PreprocessSuffix) "task/drvMisc.c"

$(IntermediateDirectory)/task_mode.c$(ObjectSuffix): task/mode.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/mode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_mode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_mode.c$(PreprocessSuffix): task/mode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_mode.c$(PreprocessSuffix) "task/mode.c"

$(IntermediateDirectory)/task_sysmem.c$(ObjectSuffix): task/sysmem.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/sysmem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_sysmem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_sysmem.c$(PreprocessSuffix): task/sysmem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_sysmem.c$(PreprocessSuffix) "task/sysmem.c"

$(IntermediateDirectory)/task_wifi_cfg.c$(ObjectSuffix): task/wifi_cfg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/wifi_cfg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_wifi_cfg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_wifi_cfg.c$(PreprocessSuffix): task/wifi_cfg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_wifi_cfg.c$(PreprocessSuffix) "task/wifi_cfg.c"

$(IntermediateDirectory)/task_main.c$(ObjectSuffix): task/main.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_main.c$(PreprocessSuffix): task/main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_main.c$(PreprocessSuffix) "task/main.c"

$(IntermediateDirectory)/task_taskManage.c$(ObjectSuffix): task/taskManage.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/taskManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_taskManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_taskManage.c$(PreprocessSuffix): task/taskManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_taskManage.c$(PreprocessSuffix) "task/taskManage.c"

$(IntermediateDirectory)/task_funcUsbDevMsg.c$(ObjectSuffix): task/funcUsbDevMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcUsbDevMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcUsbDevMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcUsbDevMsg.c$(PreprocessSuffix): task/funcUsbDevMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcUsbDevMsg.c$(PreprocessSuffix) "task/funcUsbDevMsg.c"

$(IntermediateDirectory)/task_funcTakePhoto.c$(ObjectSuffix): task/funcTakePhoto.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcTakePhoto.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcTakePhoto.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcTakePhoto.c$(PreprocessSuffix): task/funcTakePhoto.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcTakePhoto.c$(PreprocessSuffix) "task/funcTakePhoto.c"

$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(ObjectSuffix): task/funcTakePhotoMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcTakePhotoMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcTakePhotoMsg.c$(PreprocessSuffix): task/funcTakePhotoMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcTakePhotoMsg.c$(PreprocessSuffix) "task/funcTakePhotoMsg.c"

$(IntermediateDirectory)/task_uiUpgradeMsg.c$(ObjectSuffix): task/uiUpgradeMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiUpgradeMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiUpgradeMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiUpgradeMsg.c$(PreprocessSuffix): task/uiUpgradeMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiUpgradeMsg.c$(PreprocessSuffix) "task/uiUpgradeMsg.c"

$(IntermediateDirectory)/task_funcAudioRecord.c$(ObjectSuffix): task/funcAudioRecord.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcAudioRecord.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioRecord.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioRecord.c$(PreprocessSuffix): task/funcAudioRecord.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioRecord.c$(PreprocessSuffix) "task/funcAudioRecord.c"

$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(ObjectSuffix): task/funcAudioRecordMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcAudioRecordMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcAudioRecordMsg.c$(PreprocessSuffix): task/funcAudioRecordMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcAudioRecordMsg.c$(PreprocessSuffix) "task/funcAudioRecordMsg.c"

$(IntermediateDirectory)/task_menuXml.c$(ObjectSuffix): task/menuXml.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/menuXml.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_menuXml.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_menuXml.c$(PreprocessSuffix): task/menuXml.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_menuXml.c$(PreprocessSuffix) "task/menuXml.c"

$(IntermediateDirectory)/task_funMainMenu.c$(ObjectSuffix): task/funMainMenu.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu.c$(PreprocessSuffix): task/funMainMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu.c$(PreprocessSuffix) "task/funMainMenu.c"

$(IntermediateDirectory)/task_funMainMenuMsg.c$(ObjectSuffix): task/funMainMenuMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenuMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenuMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenuMsg.c$(PreprocessSuffix): task/funMainMenuMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenuMsg.c$(PreprocessSuffix) "task/funMainMenuMsg.c"

$(IntermediateDirectory)/task_funMainMenuRes.c$(ObjectSuffix): task/funMainMenuRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenuRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenuRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenuRes.c$(PreprocessSuffix): task/funMainMenuRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenuRes.c$(PreprocessSuffix) "task/funMainMenuRes.c"

$(IntermediateDirectory)/task_picSticker.c$(ObjectSuffix): task/picSticker.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/picSticker.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_picSticker.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_picSticker.c$(PreprocessSuffix): task/picSticker.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_picSticker.c$(PreprocessSuffix) "task/picSticker.c"

$(IntermediateDirectory)/task_userMedia.c$(ObjectSuffix): task/userMedia.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/userMedia.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_userMedia.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_userMedia.c$(PreprocessSuffix): task/userMedia.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_userMedia.c$(PreprocessSuffix) "task/userMedia.c"

$(IntermediateDirectory)/task_image_watermark.c$(ObjectSuffix): task/image_watermark.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/image_watermark.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_image_watermark.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_image_watermark.c$(PreprocessSuffix): task/image_watermark.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_image_watermark.c$(PreprocessSuffix) "task/image_watermark.c"

$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(ObjectSuffix): task/funcPhotoScoreMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcPhotoScoreMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(PreprocessSuffix): task/funcPhotoScoreMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPhotoScoreMsg.c$(PreprocessSuffix) "task/funcPhotoScoreMsg.c"

$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(ObjectSuffix): task/funcPhotoScoreRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcPhotoScoreRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPhotoScoreRes.c$(PreprocessSuffix): task/funcPhotoScoreRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPhotoScoreRes.c$(PreprocessSuffix) "task/funcPhotoScoreRes.c"

$(IntermediateDirectory)/task_funcPhotoScore.c$(ObjectSuffix): task/funcPhotoScore.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcPhotoScore.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcPhotoScore.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcPhotoScore.c$(PreprocessSuffix): task/funcPhotoScore.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcPhotoScore.c$(PreprocessSuffix) "task/funcPhotoScore.c"

$(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix): task/sd_config.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/sd_config.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_sd_config.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_sd_config.c$(PreprocessSuffix): task/sd_config.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_sd_config.c$(PreprocessSuffix) "task/sd_config.c"

$(IntermediateDirectory)/task_funMainMenu2.c$(ObjectSuffix): task/funMainMenu2.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu2.c$(PreprocessSuffix): task/funMainMenu2.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu2.c$(PreprocessSuffix) "task/funMainMenu2.c"

$(IntermediateDirectory)/task_funMainMenu2Msg.c$(ObjectSuffix): task/funMainMenu2Msg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu2Msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu2Msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu2Msg.c$(PreprocessSuffix): task/funMainMenu2Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu2Msg.c$(PreprocessSuffix) "task/funMainMenu2Msg.c"

$(IntermediateDirectory)/task_funMainMenu2Res.c$(ObjectSuffix): task/funMainMenu2Res.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu2Res.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu2Res.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu2Res.c$(PreprocessSuffix): task/funMainMenu2Res.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu2Res.c$(PreprocessSuffix) "task/funMainMenu2Res.c"

$(IntermediateDirectory)/task_funMainMenu3.c$(ObjectSuffix): task/funMainMenu3.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu3.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu3.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu3.c$(PreprocessSuffix): task/funMainMenu3.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu3.c$(PreprocessSuffix) "task/funMainMenu3.c"

$(IntermediateDirectory)/task_funMainMenu3Msg.c$(ObjectSuffix): task/funMainMenu3Msg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu3Msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu3Msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu3Msg.c$(PreprocessSuffix): task/funMainMenu3Msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu3Msg.c$(PreprocessSuffix) "task/funMainMenu3Msg.c"

$(IntermediateDirectory)/task_funMainMenu3Res.c$(ObjectSuffix): task/funMainMenu3Res.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funMainMenu3Res.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funMainMenu3Res.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funMainMenu3Res.c$(PreprocessSuffix): task/funMainMenu3Res.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funMainMenu3Res.c$(PreprocessSuffix) "task/funMainMenu3Res.c"

$(IntermediateDirectory)/task_funcljfNetTypeMenu.c$(ObjectSuffix): task/funcljfNetTypeMenu.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcljfNetTypeMenu.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcljfNetTypeMenu.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcljfNetTypeMenu.c$(PreprocessSuffix): task/funcljfNetTypeMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcljfNetTypeMenu.c$(PreprocessSuffix) "task/funcljfNetTypeMenu.c"

$(IntermediateDirectory)/task_funcljfNetTypeMsg.c$(ObjectSuffix): task/funcljfNetTypeMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funcljfNetTypeMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funcljfNetTypeMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funcljfNetTypeMsg.c$(PreprocessSuffix): task/funcljfNetTypeMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funcljfNetTypeMsg.c$(PreprocessSuffix) "task/funcljfNetTypeMsg.c"

$(IntermediateDirectory)/task_wifiProc.c$(ObjectSuffix): task/wifiProc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/wifiProc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_wifiProc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_wifiProc.c$(PreprocessSuffix): task/wifiProc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_wifiProc.c$(PreprocessSuffix) "task/wifiProc.c"

$(IntermediateDirectory)/task_iperf2_adapt.c$(ObjectSuffix): task/iperf2_adapt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/iperf2_adapt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_iperf2_adapt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_iperf2_adapt.c$(PreprocessSuffix): task/iperf2_adapt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_iperf2_adapt.c$(PreprocessSuffix) "task/iperf2_adapt.c"

$(IntermediateDirectory)/task_uiPhotodetailMsg.c$(ObjectSuffix): task/uiPhotodetailMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiPhotodetailMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiPhotodetailMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiPhotodetailMsg.c$(PreprocessSuffix): task/uiPhotodetailMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiPhotodetailMsg.c$(PreprocessSuffix) "task/uiPhotodetailMsg.c"

$(IntermediateDirectory)/task_uiPhotodetailRes.c$(ObjectSuffix): task/uiPhotodetailRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiPhotodetailRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiPhotodetailRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiPhotodetailRes.c$(PreprocessSuffix): task/uiPhotodetailRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiPhotodetailRes.c$(PreprocessSuffix) "task/uiPhotodetailRes.c"

$(IntermediateDirectory)/task_aiAudioDownload.c$(ObjectSuffix): task/aiAudioDownload.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/aiAudioDownload.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_aiAudioDownload.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_aiAudioDownload.c$(PreprocessSuffix): task/aiAudioDownload.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_aiAudioDownload.c$(PreprocessSuffix) "task/aiAudioDownload.c"

$(IntermediateDirectory)/task_funRoleSwitch.c$(ObjectSuffix): task/funRoleSwitch.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funRoleSwitch.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funRoleSwitch.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funRoleSwitch.c$(PreprocessSuffix): task/funRoleSwitch.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funRoleSwitch.c$(PreprocessSuffix) "task/funRoleSwitch.c"

$(IntermediateDirectory)/task_funRoleSwitchMsg.c$(ObjectSuffix): task/funRoleSwitchMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funRoleSwitchMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funRoleSwitchMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funRoleSwitchMsg.c$(PreprocessSuffix): task/funRoleSwitchMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funRoleSwitchMsg.c$(PreprocessSuffix) "task/funRoleSwitchMsg.c"

$(IntermediateDirectory)/task_funRoleSwitchRes.c$(ObjectSuffix): task/funRoleSwitchRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funRoleSwitchRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funRoleSwitchRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funRoleSwitchRes.c$(PreprocessSuffix): task/funRoleSwitchRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funRoleSwitchRes.c$(PreprocessSuffix) "task/funRoleSwitchRes.c"

$(IntermediateDirectory)/task_uiSmallModelMsg.c$(ObjectSuffix): task/uiSmallModelMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiSmallModelMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiSmallModelMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiSmallModelMsg.c$(PreprocessSuffix): task/uiSmallModelMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiSmallModelMsg.c$(PreprocessSuffix) "task/uiSmallModelMsg.c"

$(IntermediateDirectory)/task_uiSmallModelRes.c$(ObjectSuffix): task/uiSmallModelRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiSmallModelRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiSmallModelRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiSmallModelRes.c$(PreprocessSuffix): task/uiSmallModelRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiSmallModelRes.c$(PreprocessSuffix) "task/uiSmallModelRes.c"

$(IntermediateDirectory)/task_uiMenuVersionRes.c$(ObjectSuffix): task/uiMenuVersionRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuVersionRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuVersionRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuVersionRes.c$(PreprocessSuffix): task/uiMenuVersionRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuVersionRes.c$(PreprocessSuffix) "task/uiMenuVersionRes.c"

$(IntermediateDirectory)/task_funApp.c$(ObjectSuffix): task/funApp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funApp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funApp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funApp.c$(PreprocessSuffix): task/funApp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funApp.c$(PreprocessSuffix) "task/funApp.c"

$(IntermediateDirectory)/task_funAppMsg.c$(ObjectSuffix): task/funAppMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funAppMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funAppMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funAppMsg.c$(PreprocessSuffix): task/funAppMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funAppMsg.c$(PreprocessSuffix) "task/funAppMsg.c"

$(IntermediateDirectory)/task_funAppRes.c$(ObjectSuffix): task/funAppRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/funAppRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_funAppRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_funAppRes.c$(PreprocessSuffix): task/funAppRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_funAppRes.c$(PreprocessSuffix) "task/funAppRes.c"

$(IntermediateDirectory)/task_uiMenuVersionRes.c$(ObjectSuffix): task/uiMenuVersionRes.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/uiMenuVersionRes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/task_uiMenuVersionRes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/task_uiMenuVersionRes.c$(PreprocessSuffix): task/uiMenuVersionRes.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/task_uiMenuVersionRes.c$(PreprocessSuffix) "task/uiMenuVersionRes.c"

$(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix): resource/user_icon.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/resource/user_icon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/resource_user_icon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/resource_user_icon.c$(PreprocessSuffix): resource/user_icon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/resource_user_icon.c$(PreprocessSuffix) "resource/user_icon.c"

$(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix): resource/user_str.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/resource/user_str.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/resource_user_str.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/resource_user_str.c$(PreprocessSuffix): resource/user_str.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/resource_user_str.c$(PreprocessSuffix) "resource/user_str.c"

$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix): ../device/lcd/lcd_mcu_5420.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_5420.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_5420.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_5420.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_5420.c"

$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix): ../device/lcd/lcd_mcu_9225G.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_9225G.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_9225G.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_9225G.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_9225G.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix): ../device/lcd/lcd_mcu_hx8352b.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_hx8352b.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_hx8352b.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352b.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_hx8352b.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix): ../device/lcd/lcd_mcu_hx8352c.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_hx8352c.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_hx8352c.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_hx8352c.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_hx8352c.c"

$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix): ../device/lcd/lcd_mcu_hx8357b.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_hx8357b.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_hx8357b.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_hx8357b.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_hx8357b.c"

$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix): ../device/lcd/lcd_mcu_ili9328.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_ili9328.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_ili9328.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_ili9328.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_ili9328.c"

$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix): ../device/lcd/lcd_mcu_lgdp4532.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_lgdp4532.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_lgdp4532.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_lgdp4532.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_lgdp4532.c"

$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix): ../device/lcd/lcd_mcu_r61509v.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_r61509v.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_r61509v.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_r61509v.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_r61509v.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7789.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_st7789.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7789.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7789.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7789.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7789v.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_st7789v.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7789v.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7789v.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7789v.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ili8961.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_ili8961.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ili8961.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ili8961.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ili8961.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ili9342c.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_ili9342c.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ili9342c.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ili9342c.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ili9342c.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ili9806e.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_ili9806e.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ili9806e.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ili9806e.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ili9806e.c"

$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix): ../device/lcd/lcd_rgb_jl_lcd.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_jl_lcd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_jl_lcd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_jl_lcd.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_jl_lcd.c"

$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix): ../device/lcd/lcd_rgb_ota5182.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_ota5182.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_ota5182.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_ota5182.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_ota5182.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7282.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_st7282.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7282.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7282.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7282.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7701.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_st7701.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7701.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7701.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7701.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7701_HSD4.95.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_st7701_HSD4.95.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7701_HSD4.95.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7701_HSD4.95.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7701_HSD4.95.c"

$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix): ../device/lcd/lcd_rgb_9503v.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_9503v.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_9503v.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_9503v.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_9503v.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7735S_spimode.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_st7735S_spimode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7735S_spimode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7735S_spimode.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7735S_spimode.c"

$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix): ../device/lcd/lcd_mcu_st7789V_spimode.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_st7789V_spimode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_st7789V_spimode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_st7789V_spimode.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_st7789V_spimode.c"

$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix): ../device/lcd/lcd_rgb_st7701S.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_st7701S.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_st7701S.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_st7701S.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_st7701S.c"

$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix): ../device/lcd/lcd_rgb_9503.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_rgb_9503.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(PreprocessSuffix): ../device/lcd/lcd_rgb_9503.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_rgb_9503.c$(PreprocessSuffix) "../device/lcd/lcd_rgb_9503.c"

$(IntermediateDirectory)/lcd_lcd_mcu_nv3030b.c$(ObjectSuffix): ../device/lcd/lcd_mcu_nv3030b.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/lcd/lcd_mcu_nv3030b.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcd_mcu_nv3030b.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcd_mcu_nv3030b.c$(PreprocessSuffix): ../device/lcd/lcd_mcu_nv3030b.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcd_mcu_nv3030b.c$(PreprocessSuffix) "../device/lcd/lcd_mcu_nv3030b.c"

$(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix): ../device/sensor/COMMON_table.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/COMMON_table.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_COMMON_table.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_COMMON_table.c$(PreprocessSuffix): ../device/sensor/COMMON_table.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_COMMON_table.c$(PreprocessSuffix) "../device/sensor/COMMON_table.c"

$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix): ../device/sensor/SENSOR_BF2013.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_BF2013.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF2013.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF2013.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF2013.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix): ../device/sensor/SENSOR_BF3016.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_BF3016.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF3016.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF3016.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF3016.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix): ../device/sensor/SENSOR_BF3703.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_BF3703.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF3703.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF3703.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF3703.c"

$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix): ../device/sensor/SENSOR_BF3a03.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_BF3a03.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF3a03.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF3a03.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF3a03.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0307.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC0307.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0307.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0307.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0307.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0308.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC0308.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0308.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0308.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0308.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0309.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC0309.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0309.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0309.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0309.c"

$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix): ../device/sensor/SENSOR_GC0328.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC0328.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC0328.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC0328.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC0328.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1004.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC1004.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1004.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1004.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1004.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1034.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC1034.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1034.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1034.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1034.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1054.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC1054.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1054.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1054.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1054.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1064.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC1064.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1064.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1064.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1064.c"

$(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix): ../device/sensor/SENSOR_H42.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_H42.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H42.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H42.c$(PreprocessSuffix): ../device/sensor/SENSOR_H42.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H42.c$(PreprocessSuffix) "../device/sensor/SENSOR_H42.c"

$(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix): ../device/sensor/SENSOR_H62.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_H62.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H62.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H62.c$(PreprocessSuffix): ../device/sensor/SENSOR_H62.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H62.c$(PreprocessSuffix) "../device/sensor/SENSOR_H62.c"

$(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix): ../device/sensor/SENSOR_H65.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_H65.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H65.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H65.c$(PreprocessSuffix): ../device/sensor/SENSOR_H65.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H65.c$(PreprocessSuffix) "../device/sensor/SENSOR_H65.c"

$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix): ../device/sensor/SENSOR_H7640.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_H7640.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H7640.c$(PreprocessSuffix): ../device/sensor/SENSOR_H7640.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H7640.c$(PreprocessSuffix) "../device/sensor/SENSOR_H7640.c"

$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix): ../device/sensor/SENSOR_HM1055.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_HM1055.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(PreprocessSuffix): ../device/sensor/SENSOR_HM1055.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_HM1055.c$(PreprocessSuffix) "../device/sensor/SENSOR_HM1055.c"

$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix): ../device/sensor/SENSOR_IT03A1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_IT03A1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(PreprocessSuffix): ../device/sensor/SENSOR_IT03A1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_IT03A1.c$(PreprocessSuffix) "../device/sensor/SENSOR_IT03A1.c"

$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix): ../device/sensor/SENSOR_NT99141.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_NT99141.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(PreprocessSuffix): ../device/sensor/SENSOR_NT99141.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_NT99141.c$(PreprocessSuffix) "../device/sensor/SENSOR_NT99141.c"

$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix): ../device/sensor/SENSOR_NT99142.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_NT99142.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(PreprocessSuffix): ../device/sensor/SENSOR_NT99142.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_NT99142.c$(PreprocessSuffix) "../device/sensor/SENSOR_NT99142.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix): ../device/sensor/SENSOR_OV7670.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_OV7670.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV7670.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV7670.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV7670.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix): ../device/sensor/SENSOR_OV7725.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_OV7725.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV7725.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV7725.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV7725.c"

$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix): ../device/sensor/SENSOR_OV7736.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_OV7736.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV7736.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV7736.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV7736.c"

$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix): ../device/sensor/SENSOR_OV9710.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_OV9710.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV9710.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV9710.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV9710.c"

$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix): ../device/sensor/SENSOR_OV9732.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_OV9732.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(PreprocessSuffix): ../device/sensor/SENSOR_OV9732.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_OV9732.c$(PreprocessSuffix) "../device/sensor/SENSOR_OV9732.c"

$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix): ../device/sensor/SENSOR_SC1045.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SC1045.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(PreprocessSuffix): ../device/sensor/SENSOR_SC1045.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SC1045.c$(PreprocessSuffix) "../device/sensor/SENSOR_SC1045.c"

$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix): ../device/sensor/SENSOR_SC1243.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SC1243.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(PreprocessSuffix): ../device/sensor/SENSOR_SC1243.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SC1243.c$(PreprocessSuffix) "../device/sensor/SENSOR_SC1243.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix): ../device/sensor/SENSOR_SIV100B.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SIV100B.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(PreprocessSuffix): ../device/sensor/SENSOR_SIV100B.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SIV100B.c$(PreprocessSuffix) "../device/sensor/SENSOR_SIV100B.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix): ../device/sensor/SENSOR_SIV120B.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SIV120B.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(PreprocessSuffix): ../device/sensor/SENSOR_SIV120B.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SIV120B.c$(PreprocessSuffix) "../device/sensor/SENSOR_SIV120B.c"

$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix): ../device/sensor/SENSOR_SIV121DS.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SIV121DS.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(PreprocessSuffix): ../device/sensor/SENSOR_SIV121DS.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SIV121DS.c$(PreprocessSuffix) "../device/sensor/SENSOR_SIV121DS.c"

$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix): ../device/sensor/SENSOR_SP1409.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SP1409.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(PreprocessSuffix): ../device/sensor/SENSOR_SP1409.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SP1409.c$(PreprocessSuffix) "../device/sensor/SENSOR_SP1409.c"

$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix): ../device/sensor/SENSOR_SP140A.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SP140A.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(PreprocessSuffix): ../device/sensor/SENSOR_SP140A.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SP140A.c$(PreprocessSuffix) "../device/sensor/SENSOR_SP140A.c"

$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix): ../device/sensor/SNESOR_FPX1002.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SNESOR_FPX1002.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(PreprocessSuffix): ../device/sensor/SNESOR_FPX1002.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SNESOR_FPX1002.c$(PreprocessSuffix) "../device/sensor/SNESOR_FPX1002.c"

$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix): ../device/sensor/SENSOR_GC1084.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_GC1084.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(PreprocessSuffix): ../device/sensor/SENSOR_GC1084.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_GC1084.c$(PreprocessSuffix) "../device/sensor/SENSOR_GC1084.c"

$(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix): ../device/sensor/SENSOR_H63.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_H63.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H63.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H63.c$(PreprocessSuffix): ../device/sensor/SENSOR_H63.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H63.c$(PreprocessSuffix) "../device/sensor/SENSOR_H63.c"

$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix): ../device/sensor/SENSOR_SC2331.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_SC2331.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(PreprocessSuffix): ../device/sensor/SENSOR_SC2331.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_SC2331.c$(PreprocessSuffix) "../device/sensor/SENSOR_SC2331.c"

$(IntermediateDirectory)/sensor_SENSOR_BF20A6.c$(ObjectSuffix): ../device/sensor/SENSOR_BF20A6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_BF20A6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_BF20A6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_BF20A6.c$(PreprocessSuffix): ../device/sensor/SENSOR_BF20A6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_BF20A6.c$(PreprocessSuffix) "../device/sensor/SENSOR_BF20A6.c"

$(IntermediateDirectory)/sensor_SENSOR_H63P.c$(ObjectSuffix): ../device/sensor/SENSOR_H63P.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/sensor/SENSOR_H63P.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_SENSOR_H63P.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_SENSOR_H63P.c$(PreprocessSuffix): ../device/sensor/SENSOR_H63P.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_SENSOR_H63P.c$(PreprocessSuffix) "../device/sensor/SENSOR_H63P.c"

$(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix): ../device/tp/icn85xx.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/tp/icn85xx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_icn85xx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_icn85xx.c$(PreprocessSuffix): ../device/tp/icn85xx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_icn85xx.c$(PreprocessSuffix) "../device/tp/icn85xx.c"

$(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix): ../device/tp/icn85xx_flash.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/tp/icn85xx_flash.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_icn85xx_flash.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_icn85xx_flash.c$(PreprocessSuffix): ../device/tp/icn85xx_flash.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_icn85xx_flash.c$(PreprocessSuffix) "../device/tp/icn85xx_flash.c"

$(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix): ../device/tp/icnt81.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/tp/icnt81.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_icnt81.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_icnt81.c$(PreprocessSuffix): ../device/tp/icnt81.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_icnt81.c$(PreprocessSuffix) "../device/tp/icnt81.c"

$(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix): ../device/tp/touchPanel.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/tp/touchPanel.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_touchPanel.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_touchPanel.c$(PreprocessSuffix): ../device/tp/touchPanel.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_touchPanel.c$(PreprocessSuffix) "../device/tp/touchPanel.c"

$(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix): ../device/tp/tpiic.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/device/tp/tpiic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp_tpiic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp_tpiic.c$(PreprocessSuffix): ../device/tp/tpiic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp_tpiic.c$(PreprocessSuffix) "../device/tp/tpiic.c"

$(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix): ../../board/AX3292/board.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/board/AX3292/board.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_board.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_board.c$(PreprocessSuffix): ../../board/AX3292/board.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_board.c$(PreprocessSuffix) "../../board/AX3292/board.c"

$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix): ../../board/AX3292/build-nd.s 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/board/AX3292/build-nd.s" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_build-nd.s$(PreprocessSuffix): ../../board/AX3292/build-nd.s
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_build-nd.s$(PreprocessSuffix) "../../board/AX3292/build-nd.s"

$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix): ../../board/AX3292/spi_boot_cfg.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/board/AX3292/spi_boot_cfg.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(PreprocessSuffix): ../../board/AX3292/spi_boot_cfg.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_spi_boot_cfg.S$(PreprocessSuffix) "../../board/AX3292/spi_boot_cfg.S"

$(IntermediateDirectory)/game_funcGame.c$(ObjectSuffix): task/game/funcGame.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/funcGame.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_funcGame.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_funcGame.c$(PreprocessSuffix): task/game/funcGame.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_funcGame.c$(PreprocessSuffix) "task/game/funcGame.c"

$(IntermediateDirectory)/game_funcGameMenu.c$(ObjectSuffix): task/game/funcGameMenu.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/funcGameMenu.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_funcGameMenu.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_funcGameMenu.c$(PreprocessSuffix): task/game/funcGameMenu.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_funcGameMenu.c$(PreprocessSuffix) "task/game/funcGameMenu.c"

$(IntermediateDirectory)/game_funcGameMenuMsg.c$(ObjectSuffix): task/game/funcGameMenuMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/funcGameMenuMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_funcGameMenuMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_funcGameMenuMsg.c$(PreprocessSuffix): task/game/funcGameMenuMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_funcGameMenuMsg.c$(PreprocessSuffix) "task/game/funcGameMenuMsg.c"

$(IntermediateDirectory)/game_funcGameMsg.c$(ObjectSuffix): task/game/funcGameMsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/funcGameMsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_funcGameMsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_funcGameMsg.c$(PreprocessSuffix): task/game/funcGameMsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_funcGameMsg.c$(PreprocessSuffix) "task/game/funcGameMsg.c"

$(IntermediateDirectory)/game_game_block.c$(ObjectSuffix): task/game/game_block.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/game_block.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_block.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_block.c$(PreprocessSuffix): task/game/game_block.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_block.c$(PreprocessSuffix) "task/game/game_block.c"

$(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix): task/game/game_maze.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/game_maze.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_maze.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_maze.c$(PreprocessSuffix): task/game/game_maze.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_maze.c$(PreprocessSuffix) "task/game/game_maze.c"

$(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix): task/game/game_snake.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/game_snake.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_snake.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_snake.c$(PreprocessSuffix): task/game/game_snake.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_snake.c$(PreprocessSuffix) "task/game/game_snake.c"

$(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix): task/game/game_sokoban.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/game_sokoban.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_sokoban.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_sokoban.c$(PreprocessSuffix): task/game/game_sokoban.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_sokoban.c$(PreprocessSuffix) "task/game/game_sokoban.c"

$(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix): task/game/game_tetris.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/demo/helloftv/task/game/game_tetris.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/game_game_tetris.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/game_game_tetris.c$(PreprocessSuffix): task/game/game_tetris.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/game_game_tetris.c$(PreprocessSuffix) "task/game/game_tetris.c"

##
## Clean
##
clean:
	$(RM) -r ./Debug/


