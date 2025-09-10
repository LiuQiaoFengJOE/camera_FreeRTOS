##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-sys-sv6158
ConfigurationName      :=Debug
WorkspacePath          := "E:\proj_code\WIFI_YG-K2T-TC\firmware\sdk\build"
ProjectPath            := "E:\proj_code\WIFI_YG-K2T-TC\firmware\sdk\AX3292"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=11
Date                   :=3/28/2025
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
OutputFile             :=../lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)DEBUG= 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="AX3292-sys-sv6158.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../lib $(IncludeSwitch)../../lib/drv $(IncludeSwitch)../../sys $(IncludeSwitch)../../inc $(IncludeSwitch)../../net/ssv6158/host/include $(IncludeSwitch)../../net/ssv6158/host/include/priv $(IncludeSwitch)../../net/ssv6158/host/include/priv/hw $(IncludeSwitch)../../net/ssv6158/host/netstack_wrapper/icomm_lwIP $(IncludeSwitch)../../net/ssv6158/host/os_wrapper/FreeRTOS $(IncludeSwitch)../../net/ssv6158/host/platform/jianrong $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/src $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/src/arch/ $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/src/include/ $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/src/include/ipv4 $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/src/include/ipv6 $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/ports/icomm $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/ports/icomm/include $(IncludeSwitch)../../net/ssv6158/host/tcpip/lwip-1.4.0/ports/icomm/include/arch $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../arch/ $(IncludeSwitch)../../arch/AX3292/hal/inc $(IncludeSwitch)../../arch/AX3292/ax32xx/inc $(IncludeSwitch)../../sys/drv $(IncludeSwitch)../../board/AX3292/mvl8801 $(IncludeSwitch)../../demo/helloftv $(IncludeSwitch)../../demo/helloftv/res $(IncludeSwitch)../../arch/AX3292/inc/asm 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)C:/MinGW-4.7.1/lib/gcc/or1k-elf/4.9.1/no-delay $(LibraryPathSwitch)C:/MinGW-4.7.1/or1k-elf/lib/no-delay  $(LibraryPathSwitch). 

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
Objects0=$(IntermediateDirectory)/auadc_auadc.c$(ObjectSuffix) $(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) $(IntermediateDirectory)/config_conf_parse.c$(ObjectSuffix) $(IntermediateDirectory)/config_conf_scan.c$(ObjectSuffix) $(IntermediateDirectory)/config_scan.c$(ObjectSuffix) $(IntermediateDirectory)/config_scan1.c$(ObjectSuffix) $(IntermediateDirectory)/eloop_eloop.c$(ObjectSuffix) $(IntermediateDirectory)/eloop_irqTimer.c$(ObjectSuffix) $(IntermediateDirectory)/eventTimer_eventTimer.c$(ObjectSuffix) $(IntermediateDirectory)/fly_fly.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ftp_ftp-fs.c$(ObjectSuffix) $(IntermediateDirectory)/ftp_ftp.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_default.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num2.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num3.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num4.c$(ObjectSuffix) \
	$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num5.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_hal_watermark.c$(ObjectSuffix) $(IntermediateDirectory)/hal_watermark_watermark_stream.c$(ObjectSuffix) $(IntermediateDirectory)/http_http.c$(ObjectSuffix) $(IntermediateDirectory)/mac_fd_mac_fd.c$(ObjectSuffix) $(IntermediateDirectory)/mediaFile_playVideo.c$(ObjectSuffix) $(IntermediateDirectory)/mediaFile_record.c$(ObjectSuffix) $(IntermediateDirectory)/mediaFile_playNet.c$(ObjectSuffix) \
	$(IntermediateDirectory)/mediaFile_readFile.c$(ObjectSuffix) $(IntermediateDirectory)/printf_printf.c$(ObjectSuffix) $(IntermediateDirectory)/printf_vsprintf.c$(ObjectSuffix) $(IntermediateDirectory)/printf_vsscanf.c$(ObjectSuffix) $(IntermediateDirectory)/resfs_resfs.c$(ObjectSuffix) $(IntermediateDirectory)/rtp-media_rtp-audio.c$(ObjectSuffix) $(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(ObjectSuffix) $(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_button.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_dialog.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sUI_frameWin.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_imageIcon.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_itemManage.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_memPool.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_menuItem.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_menuItemEx.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_menuOption.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_progressBar.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_rect.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_stringIcon.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/spi_spi.c$(ObjectSuffix) $(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix) $(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix) $(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix) $(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix) \
	$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix) $(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix) $(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix) $(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix) $(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix) \
	$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix) $(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix) $(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix) $(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix) $(IntermediateDirectory)/printer_printer.c$(ObjectSuffix) $(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix) $(IntermediateDirectory)/csi_common.c$(ObjectSuffix) $(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix) $(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix) $(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix) \
	$(IntermediateDirectory)/dac_dac.c$(ObjectSuffix) $(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix) $(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix) $(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix) \
	$(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix) $(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "E:\proj_code\WIFI_YG-K2T-TC\firmware\sdk\build/.build-debug"
	@echo rebuilt > "E:\proj_code\WIFI_YG-K2T-TC\firmware\sdk\build/.build-debug/AX3292-sys-sv6158"

./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/auadc_auadc.c$(ObjectSuffix): ../../sys/auadc/auadc.c $(IntermediateDirectory)/auadc_auadc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/auadc/auadc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/auadc_auadc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/auadc_auadc.c$(DependSuffix): ../../sys/auadc/auadc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/auadc_auadc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/auadc_auadc.c$(DependSuffix) -MM "../../sys/auadc/auadc.c"

$(IntermediateDirectory)/auadc_auadc.c$(PreprocessSuffix): ../../sys/auadc/auadc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/auadc_auadc.c$(PreprocessSuffix) "../../sys/auadc/auadc.c"

$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix): ../../sys/cjson/cJSON.c $(IntermediateDirectory)/cjson_cJSON.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/cjson/cJSON.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cjson_cJSON.c$(DependSuffix): ../../sys/cjson/cJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) -MF$(IntermediateDirectory)/cjson_cJSON.c$(DependSuffix) -MM "../../sys/cjson/cJSON.c"

$(IntermediateDirectory)/cjson_cJSON.c$(PreprocessSuffix): ../../sys/cjson/cJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cjson_cJSON.c$(PreprocessSuffix) "../../sys/cjson/cJSON.c"

$(IntermediateDirectory)/config_conf_parse.c$(ObjectSuffix): ../../sys/config/conf_parse.c $(IntermediateDirectory)/config_conf_parse.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/config/conf_parse.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_conf_parse.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_conf_parse.c$(DependSuffix): ../../sys/config/conf_parse.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/config_conf_parse.c$(ObjectSuffix) -MF$(IntermediateDirectory)/config_conf_parse.c$(DependSuffix) -MM "../../sys/config/conf_parse.c"

$(IntermediateDirectory)/config_conf_parse.c$(PreprocessSuffix): ../../sys/config/conf_parse.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_conf_parse.c$(PreprocessSuffix) "../../sys/config/conf_parse.c"

$(IntermediateDirectory)/config_conf_scan.c$(ObjectSuffix): ../../sys/config/conf_scan.c $(IntermediateDirectory)/config_conf_scan.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/config/conf_scan.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_conf_scan.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_conf_scan.c$(DependSuffix): ../../sys/config/conf_scan.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/config_conf_scan.c$(ObjectSuffix) -MF$(IntermediateDirectory)/config_conf_scan.c$(DependSuffix) -MM "../../sys/config/conf_scan.c"

$(IntermediateDirectory)/config_conf_scan.c$(PreprocessSuffix): ../../sys/config/conf_scan.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_conf_scan.c$(PreprocessSuffix) "../../sys/config/conf_scan.c"

$(IntermediateDirectory)/config_scan.c$(ObjectSuffix): ../../sys/config/scan.c $(IntermediateDirectory)/config_scan.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/config/scan.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_scan.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_scan.c$(DependSuffix): ../../sys/config/scan.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/config_scan.c$(ObjectSuffix) -MF$(IntermediateDirectory)/config_scan.c$(DependSuffix) -MM "../../sys/config/scan.c"

$(IntermediateDirectory)/config_scan.c$(PreprocessSuffix): ../../sys/config/scan.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_scan.c$(PreprocessSuffix) "../../sys/config/scan.c"

$(IntermediateDirectory)/config_scan1.c$(ObjectSuffix): ../../sys/config/scan1.c $(IntermediateDirectory)/config_scan1.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/config/scan1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_scan1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_scan1.c$(DependSuffix): ../../sys/config/scan1.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/config_scan1.c$(ObjectSuffix) -MF$(IntermediateDirectory)/config_scan1.c$(DependSuffix) -MM "../../sys/config/scan1.c"

$(IntermediateDirectory)/config_scan1.c$(PreprocessSuffix): ../../sys/config/scan1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_scan1.c$(PreprocessSuffix) "../../sys/config/scan1.c"

$(IntermediateDirectory)/eloop_eloop.c$(ObjectSuffix): ../../sys/eloop/eloop.c $(IntermediateDirectory)/eloop_eloop.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/eloop/eloop.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eloop_eloop.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eloop_eloop.c$(DependSuffix): ../../sys/eloop/eloop.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/eloop_eloop.c$(ObjectSuffix) -MF$(IntermediateDirectory)/eloop_eloop.c$(DependSuffix) -MM "../../sys/eloop/eloop.c"

$(IntermediateDirectory)/eloop_eloop.c$(PreprocessSuffix): ../../sys/eloop/eloop.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eloop_eloop.c$(PreprocessSuffix) "../../sys/eloop/eloop.c"

$(IntermediateDirectory)/eloop_irqTimer.c$(ObjectSuffix): ../../sys/eloop/irqTimer.c $(IntermediateDirectory)/eloop_irqTimer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/eloop/irqTimer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eloop_irqTimer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eloop_irqTimer.c$(DependSuffix): ../../sys/eloop/irqTimer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/eloop_irqTimer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/eloop_irqTimer.c$(DependSuffix) -MM "../../sys/eloop/irqTimer.c"

$(IntermediateDirectory)/eloop_irqTimer.c$(PreprocessSuffix): ../../sys/eloop/irqTimer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eloop_irqTimer.c$(PreprocessSuffix) "../../sys/eloop/irqTimer.c"

$(IntermediateDirectory)/eventTimer_eventTimer.c$(ObjectSuffix): ../../sys/eventTimer/eventTimer.c $(IntermediateDirectory)/eventTimer_eventTimer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/eventTimer/eventTimer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eventTimer_eventTimer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eventTimer_eventTimer.c$(DependSuffix): ../../sys/eventTimer/eventTimer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/eventTimer_eventTimer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/eventTimer_eventTimer.c$(DependSuffix) -MM "../../sys/eventTimer/eventTimer.c"

$(IntermediateDirectory)/eventTimer_eventTimer.c$(PreprocessSuffix): ../../sys/eventTimer/eventTimer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eventTimer_eventTimer.c$(PreprocessSuffix) "../../sys/eventTimer/eventTimer.c"

$(IntermediateDirectory)/fly_fly.c$(ObjectSuffix): ../../sys/fly/fly.c $(IntermediateDirectory)/fly_fly.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/fly/fly.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fly_fly.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fly_fly.c$(DependSuffix): ../../sys/fly/fly.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fly_fly.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fly_fly.c$(DependSuffix) -MM "../../sys/fly/fly.c"

$(IntermediateDirectory)/fly_fly.c$(PreprocessSuffix): ../../sys/fly/fly.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fly_fly.c$(PreprocessSuffix) "../../sys/fly/fly.c"

$(IntermediateDirectory)/ftp_ftp-fs.c$(ObjectSuffix): ../../sys/ftp/ftp-fs.c $(IntermediateDirectory)/ftp_ftp-fs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/ftp/ftp-fs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ftp_ftp-fs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ftp_ftp-fs.c$(DependSuffix): ../../sys/ftp/ftp-fs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ftp_ftp-fs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ftp_ftp-fs.c$(DependSuffix) -MM "../../sys/ftp/ftp-fs.c"

$(IntermediateDirectory)/ftp_ftp-fs.c$(PreprocessSuffix): ../../sys/ftp/ftp-fs.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ftp_ftp-fs.c$(PreprocessSuffix) "../../sys/ftp/ftp-fs.c"

$(IntermediateDirectory)/ftp_ftp.c$(ObjectSuffix): ../../sys/ftp/ftp.c $(IntermediateDirectory)/ftp_ftp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/ftp/ftp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ftp_ftp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ftp_ftp.c$(DependSuffix): ../../sys/ftp/ftp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ftp_ftp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ftp_ftp.c$(DependSuffix) -MM "../../sys/ftp/ftp.c"

$(IntermediateDirectory)/ftp_ftp.c$(PreprocessSuffix): ../../sys/ftp/ftp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ftp_ftp.c$(PreprocessSuffix) "../../sys/ftp/ftp.c"

$(IntermediateDirectory)/hal_watermark_ascii.c$(ObjectSuffix): ../../sys/hal_watermark/ascii.c $(IntermediateDirectory)/hal_watermark_ascii.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii.c$(DependSuffix): ../../sys/hal_watermark/ascii.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii.c"

$(IntermediateDirectory)/hal_watermark_ascii.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii.c"

$(IntermediateDirectory)/hal_watermark_ascii_default.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_default.c $(IntermediateDirectory)/hal_watermark_ascii_default.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_default.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_default.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_default.c$(DependSuffix): ../../sys/hal_watermark/ascii_default.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_default.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_default.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_default.c"

$(IntermediateDirectory)/hal_watermark_ascii_default.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_default.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_default.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_default.c"

$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_default_draw.c $(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_default_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(DependSuffix): ../../sys/hal_watermark/ascii_default_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_default_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_default_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_default_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num2.c $(IntermediateDirectory)/hal_watermark_ascii_num2.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(DependSuffix): ../../sys/hal_watermark/ascii_num2.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num2.c"

$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num2.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num2.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num2.c"

$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num2_draw.c $(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num2_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(DependSuffix): ../../sys/hal_watermark/ascii_num2_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num2_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num2_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num2_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num3.c $(IntermediateDirectory)/hal_watermark_ascii_num3.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num3.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(DependSuffix): ../../sys/hal_watermark/ascii_num3.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num3.c"

$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num3.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num3.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num3.c"

$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num3_draw.c $(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num3_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(DependSuffix): ../../sys/hal_watermark/ascii_num3_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num3_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num3_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num3_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num4.c $(IntermediateDirectory)/hal_watermark_ascii_num4.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(DependSuffix): ../../sys/hal_watermark/ascii_num4.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num4.c"

$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num4.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num4.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num4.c"

$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num4_draw.c $(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num4_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(DependSuffix): ../../sys/hal_watermark/ascii_num4_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num4_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num4_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num4_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num5.c $(IntermediateDirectory)/hal_watermark_ascii_num5.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(DependSuffix): ../../sys/hal_watermark/ascii_num5.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num5.c"

$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num5.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num5.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num5.c"

$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num5_draw.c $(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/ascii_num5_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(DependSuffix): ../../sys/hal_watermark/ascii_num5_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(DependSuffix) -MM "../../sys/hal_watermark/ascii_num5_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num5_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num5_draw.c"

$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(ObjectSuffix): ../../sys/hal_watermark/hal_watermark.c $(IntermediateDirectory)/hal_watermark_hal_watermark.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/hal_watermark.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(DependSuffix): ../../sys/hal_watermark/hal_watermark.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(DependSuffix) -MM "../../sys/hal_watermark/hal_watermark.c"

$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(PreprocessSuffix): ../../sys/hal_watermark/hal_watermark.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_hal_watermark.c$(PreprocessSuffix) "../../sys/hal_watermark/hal_watermark.c"

$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(ObjectSuffix): ../../sys/hal_watermark/watermark_stream.c $(IntermediateDirectory)/hal_watermark_watermark_stream.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/hal_watermark/watermark_stream.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(DependSuffix): ../../sys/hal_watermark/watermark_stream.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(DependSuffix) -MM "../../sys/hal_watermark/watermark_stream.c"

$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(PreprocessSuffix): ../../sys/hal_watermark/watermark_stream.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_watermark_stream.c$(PreprocessSuffix) "../../sys/hal_watermark/watermark_stream.c"

$(IntermediateDirectory)/http_http.c$(ObjectSuffix): ../../sys/http/http.c $(IntermediateDirectory)/http_http.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/http/http.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/http_http.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/http_http.c$(DependSuffix): ../../sys/http/http.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/http_http.c$(ObjectSuffix) -MF$(IntermediateDirectory)/http_http.c$(DependSuffix) -MM "../../sys/http/http.c"

$(IntermediateDirectory)/http_http.c$(PreprocessSuffix): ../../sys/http/http.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/http_http.c$(PreprocessSuffix) "../../sys/http/http.c"

$(IntermediateDirectory)/mac_fd_mac_fd.c$(ObjectSuffix): ../../sys/mac_fd/mac_fd.c $(IntermediateDirectory)/mac_fd_mac_fd.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/mac_fd/mac_fd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mac_fd_mac_fd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mac_fd_mac_fd.c$(DependSuffix): ../../sys/mac_fd/mac_fd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mac_fd_mac_fd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mac_fd_mac_fd.c$(DependSuffix) -MM "../../sys/mac_fd/mac_fd.c"

$(IntermediateDirectory)/mac_fd_mac_fd.c$(PreprocessSuffix): ../../sys/mac_fd/mac_fd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mac_fd_mac_fd.c$(PreprocessSuffix) "../../sys/mac_fd/mac_fd.c"

$(IntermediateDirectory)/mediaFile_playVideo.c$(ObjectSuffix): ../../sys/mediaFile/playVideo.c $(IntermediateDirectory)/mediaFile_playVideo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/mediaFile/playVideo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_playVideo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_playVideo.c$(DependSuffix): ../../sys/mediaFile/playVideo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mediaFile_playVideo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mediaFile_playVideo.c$(DependSuffix) -MM "../../sys/mediaFile/playVideo.c"

$(IntermediateDirectory)/mediaFile_playVideo.c$(PreprocessSuffix): ../../sys/mediaFile/playVideo.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_playVideo.c$(PreprocessSuffix) "../../sys/mediaFile/playVideo.c"

$(IntermediateDirectory)/mediaFile_record.c$(ObjectSuffix): ../../sys/mediaFile/record.c $(IntermediateDirectory)/mediaFile_record.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/mediaFile/record.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_record.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_record.c$(DependSuffix): ../../sys/mediaFile/record.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mediaFile_record.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mediaFile_record.c$(DependSuffix) -MM "../../sys/mediaFile/record.c"

$(IntermediateDirectory)/mediaFile_record.c$(PreprocessSuffix): ../../sys/mediaFile/record.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_record.c$(PreprocessSuffix) "../../sys/mediaFile/record.c"

$(IntermediateDirectory)/mediaFile_playNet.c$(ObjectSuffix): ../../sys/mediaFile/playNet.c $(IntermediateDirectory)/mediaFile_playNet.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/mediaFile/playNet.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_playNet.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_playNet.c$(DependSuffix): ../../sys/mediaFile/playNet.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mediaFile_playNet.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mediaFile_playNet.c$(DependSuffix) -MM "../../sys/mediaFile/playNet.c"

$(IntermediateDirectory)/mediaFile_playNet.c$(PreprocessSuffix): ../../sys/mediaFile/playNet.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_playNet.c$(PreprocessSuffix) "../../sys/mediaFile/playNet.c"

$(IntermediateDirectory)/mediaFile_readFile.c$(ObjectSuffix): ../../sys/mediaFile/readFile.c $(IntermediateDirectory)/mediaFile_readFile.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/mediaFile/readFile.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_readFile.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_readFile.c$(DependSuffix): ../../sys/mediaFile/readFile.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mediaFile_readFile.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mediaFile_readFile.c$(DependSuffix) -MM "../../sys/mediaFile/readFile.c"

$(IntermediateDirectory)/mediaFile_readFile.c$(PreprocessSuffix): ../../sys/mediaFile/readFile.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_readFile.c$(PreprocessSuffix) "../../sys/mediaFile/readFile.c"

$(IntermediateDirectory)/printf_printf.c$(ObjectSuffix): ../../sys/printf/printf.c $(IntermediateDirectory)/printf_printf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/printf/printf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printf_printf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printf_printf.c$(DependSuffix): ../../sys/printf/printf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/printf_printf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/printf_printf.c$(DependSuffix) -MM "../../sys/printf/printf.c"

$(IntermediateDirectory)/printf_printf.c$(PreprocessSuffix): ../../sys/printf/printf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printf_printf.c$(PreprocessSuffix) "../../sys/printf/printf.c"

$(IntermediateDirectory)/printf_vsprintf.c$(ObjectSuffix): ../../sys/printf/vsprintf.c $(IntermediateDirectory)/printf_vsprintf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/printf/vsprintf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printf_vsprintf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printf_vsprintf.c$(DependSuffix): ../../sys/printf/vsprintf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/printf_vsprintf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/printf_vsprintf.c$(DependSuffix) -MM "../../sys/printf/vsprintf.c"

$(IntermediateDirectory)/printf_vsprintf.c$(PreprocessSuffix): ../../sys/printf/vsprintf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printf_vsprintf.c$(PreprocessSuffix) "../../sys/printf/vsprintf.c"

$(IntermediateDirectory)/printf_vsscanf.c$(ObjectSuffix): ../../sys/printf/vsscanf.c $(IntermediateDirectory)/printf_vsscanf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/printf/vsscanf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printf_vsscanf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printf_vsscanf.c$(DependSuffix): ../../sys/printf/vsscanf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/printf_vsscanf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/printf_vsscanf.c$(DependSuffix) -MM "../../sys/printf/vsscanf.c"

$(IntermediateDirectory)/printf_vsscanf.c$(PreprocessSuffix): ../../sys/printf/vsscanf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printf_vsscanf.c$(PreprocessSuffix) "../../sys/printf/vsscanf.c"

$(IntermediateDirectory)/resfs_resfs.c$(ObjectSuffix): ../../sys/resfs/resfs.c $(IntermediateDirectory)/resfs_resfs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/resfs/resfs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/resfs_resfs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/resfs_resfs.c$(DependSuffix): ../../sys/resfs/resfs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/resfs_resfs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/resfs_resfs.c$(DependSuffix) -MM "../../sys/resfs/resfs.c"

$(IntermediateDirectory)/resfs_resfs.c$(PreprocessSuffix): ../../sys/resfs/resfs.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/resfs_resfs.c$(PreprocessSuffix) "../../sys/resfs/resfs.c"

$(IntermediateDirectory)/rtp-media_rtp-audio.c$(ObjectSuffix): ../../sys/rtp-media/rtp-audio.c $(IntermediateDirectory)/rtp-media_rtp-audio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/rtp-media/rtp-audio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtp-media_rtp-audio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtp-media_rtp-audio.c$(DependSuffix): ../../sys/rtp-media/rtp-audio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rtp-media_rtp-audio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/rtp-media_rtp-audio.c$(DependSuffix) -MM "../../sys/rtp-media/rtp-audio.c"

$(IntermediateDirectory)/rtp-media_rtp-audio.c$(PreprocessSuffix): ../../sys/rtp-media/rtp-audio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtp-media_rtp-audio.c$(PreprocessSuffix) "../../sys/rtp-media/rtp-audio.c"

$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(ObjectSuffix): ../../sys/rtp-media/rtp-jpeg.c $(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/rtp-media/rtp-jpeg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(DependSuffix): ../../sys/rtp-media/rtp-jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(DependSuffix) -MM "../../sys/rtp-media/rtp-jpeg.c"

$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(PreprocessSuffix): ../../sys/rtp-media/rtp-jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(PreprocessSuffix) "../../sys/rtp-media/rtp-jpeg.c"

$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(ObjectSuffix): ../../sys/rtp-media/rtp-user-jpeg.c $(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/rtp-media/rtp-user-jpeg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(DependSuffix): ../../sys/rtp-media/rtp-user-jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(DependSuffix) -MM "../../sys/rtp-media/rtp-user-jpeg.c"

$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(PreprocessSuffix): ../../sys/rtp-media/rtp-user-jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(PreprocessSuffix) "../../sys/rtp-media/rtp-user-jpeg.c"

$(IntermediateDirectory)/sUI_button.c$(ObjectSuffix): ../../sys/sUI/button.c $(IntermediateDirectory)/sUI_button.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/button.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_button.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_button.c$(DependSuffix): ../../sys/sUI/button.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_button.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_button.c$(DependSuffix) -MM "../../sys/sUI/button.c"

$(IntermediateDirectory)/sUI_button.c$(PreprocessSuffix): ../../sys/sUI/button.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_button.c$(PreprocessSuffix) "../../sys/sUI/button.c"

$(IntermediateDirectory)/sUI_dialog.c$(ObjectSuffix): ../../sys/sUI/dialog.c $(IntermediateDirectory)/sUI_dialog.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/dialog.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_dialog.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_dialog.c$(DependSuffix): ../../sys/sUI/dialog.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_dialog.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_dialog.c$(DependSuffix) -MM "../../sys/sUI/dialog.c"

$(IntermediateDirectory)/sUI_dialog.c$(PreprocessSuffix): ../../sys/sUI/dialog.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_dialog.c$(PreprocessSuffix) "../../sys/sUI/dialog.c"

$(IntermediateDirectory)/sUI_frameWin.c$(ObjectSuffix): ../../sys/sUI/frameWin.c $(IntermediateDirectory)/sUI_frameWin.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/frameWin.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_frameWin.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_frameWin.c$(DependSuffix): ../../sys/sUI/frameWin.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_frameWin.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_frameWin.c$(DependSuffix) -MM "../../sys/sUI/frameWin.c"

$(IntermediateDirectory)/sUI_frameWin.c$(PreprocessSuffix): ../../sys/sUI/frameWin.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_frameWin.c$(PreprocessSuffix) "../../sys/sUI/frameWin.c"

$(IntermediateDirectory)/sUI_imageIcon.c$(ObjectSuffix): ../../sys/sUI/imageIcon.c $(IntermediateDirectory)/sUI_imageIcon.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/imageIcon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_imageIcon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_imageIcon.c$(DependSuffix): ../../sys/sUI/imageIcon.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_imageIcon.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_imageIcon.c$(DependSuffix) -MM "../../sys/sUI/imageIcon.c"

$(IntermediateDirectory)/sUI_imageIcon.c$(PreprocessSuffix): ../../sys/sUI/imageIcon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_imageIcon.c$(PreprocessSuffix) "../../sys/sUI/imageIcon.c"

$(IntermediateDirectory)/sUI_itemManage.c$(ObjectSuffix): ../../sys/sUI/itemManage.c $(IntermediateDirectory)/sUI_itemManage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/itemManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_itemManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_itemManage.c$(DependSuffix): ../../sys/sUI/itemManage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_itemManage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_itemManage.c$(DependSuffix) -MM "../../sys/sUI/itemManage.c"

$(IntermediateDirectory)/sUI_itemManage.c$(PreprocessSuffix): ../../sys/sUI/itemManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_itemManage.c$(PreprocessSuffix) "../../sys/sUI/itemManage.c"

$(IntermediateDirectory)/sUI_memPool.c$(ObjectSuffix): ../../sys/sUI/memPool.c $(IntermediateDirectory)/sUI_memPool.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/memPool.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_memPool.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_memPool.c$(DependSuffix): ../../sys/sUI/memPool.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_memPool.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_memPool.c$(DependSuffix) -MM "../../sys/sUI/memPool.c"

$(IntermediateDirectory)/sUI_memPool.c$(PreprocessSuffix): ../../sys/sUI/memPool.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_memPool.c$(PreprocessSuffix) "../../sys/sUI/memPool.c"

$(IntermediateDirectory)/sUI_menuItem.c$(ObjectSuffix): ../../sys/sUI/menuItem.c $(IntermediateDirectory)/sUI_menuItem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/menuItem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_menuItem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_menuItem.c$(DependSuffix): ../../sys/sUI/menuItem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_menuItem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_menuItem.c$(DependSuffix) -MM "../../sys/sUI/menuItem.c"

$(IntermediateDirectory)/sUI_menuItem.c$(PreprocessSuffix): ../../sys/sUI/menuItem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_menuItem.c$(PreprocessSuffix) "../../sys/sUI/menuItem.c"

$(IntermediateDirectory)/sUI_menuItemEx.c$(ObjectSuffix): ../../sys/sUI/menuItemEx.c $(IntermediateDirectory)/sUI_menuItemEx.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/menuItemEx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_menuItemEx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_menuItemEx.c$(DependSuffix): ../../sys/sUI/menuItemEx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_menuItemEx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_menuItemEx.c$(DependSuffix) -MM "../../sys/sUI/menuItemEx.c"

$(IntermediateDirectory)/sUI_menuItemEx.c$(PreprocessSuffix): ../../sys/sUI/menuItemEx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_menuItemEx.c$(PreprocessSuffix) "../../sys/sUI/menuItemEx.c"

$(IntermediateDirectory)/sUI_menuOption.c$(ObjectSuffix): ../../sys/sUI/menuOption.c $(IntermediateDirectory)/sUI_menuOption.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/menuOption.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_menuOption.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_menuOption.c$(DependSuffix): ../../sys/sUI/menuOption.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_menuOption.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_menuOption.c$(DependSuffix) -MM "../../sys/sUI/menuOption.c"

$(IntermediateDirectory)/sUI_menuOption.c$(PreprocessSuffix): ../../sys/sUI/menuOption.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_menuOption.c$(PreprocessSuffix) "../../sys/sUI/menuOption.c"

$(IntermediateDirectory)/sUI_progressBar.c$(ObjectSuffix): ../../sys/sUI/progressBar.c $(IntermediateDirectory)/sUI_progressBar.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/progressBar.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_progressBar.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_progressBar.c$(DependSuffix): ../../sys/sUI/progressBar.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_progressBar.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_progressBar.c$(DependSuffix) -MM "../../sys/sUI/progressBar.c"

$(IntermediateDirectory)/sUI_progressBar.c$(PreprocessSuffix): ../../sys/sUI/progressBar.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_progressBar.c$(PreprocessSuffix) "../../sys/sUI/progressBar.c"

$(IntermediateDirectory)/sUI_rect.c$(ObjectSuffix): ../../sys/sUI/rect.c $(IntermediateDirectory)/sUI_rect.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/rect.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_rect.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_rect.c$(DependSuffix): ../../sys/sUI/rect.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_rect.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_rect.c$(DependSuffix) -MM "../../sys/sUI/rect.c"

$(IntermediateDirectory)/sUI_rect.c$(PreprocessSuffix): ../../sys/sUI/rect.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_rect.c$(PreprocessSuffix) "../../sys/sUI/rect.c"

$(IntermediateDirectory)/sUI_stringIcon.c$(ObjectSuffix): ../../sys/sUI/stringIcon.c $(IntermediateDirectory)/sUI_stringIcon.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/stringIcon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_stringIcon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_stringIcon.c$(DependSuffix): ../../sys/sUI/stringIcon.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_stringIcon.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_stringIcon.c$(DependSuffix) -MM "../../sys/sUI/stringIcon.c"

$(IntermediateDirectory)/sUI_stringIcon.c$(PreprocessSuffix): ../../sys/sUI/stringIcon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_stringIcon.c$(PreprocessSuffix) "../../sys/sUI/stringIcon.c"

$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix): ../../sys/sUI/tips.c $(IntermediateDirectory)/sUI_tips.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/tips.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_tips.c$(DependSuffix): ../../sys/sUI/tips.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_tips.c$(DependSuffix) -MM "../../sys/sUI/tips.c"

$(IntermediateDirectory)/sUI_tips.c$(PreprocessSuffix): ../../sys/sUI/tips.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_tips.c$(PreprocessSuffix) "../../sys/sUI/tips.c"

$(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix): ../../sys/sUI/uiAlloc.c $(IntermediateDirectory)/sUI_uiAlloc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/uiAlloc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_uiAlloc.c$(DependSuffix): ../../sys/sUI/uiAlloc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_uiAlloc.c$(DependSuffix) -MM "../../sys/sUI/uiAlloc.c"

$(IntermediateDirectory)/sUI_uiAlloc.c$(PreprocessSuffix): ../../sys/sUI/uiAlloc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_uiAlloc.c$(PreprocessSuffix) "../../sys/sUI/uiAlloc.c"

$(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix): ../../sys/sUI/widget.c $(IntermediateDirectory)/sUI_widget.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/widget.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_widget.c$(DependSuffix): ../../sys/sUI/widget.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_widget.c$(DependSuffix) -MM "../../sys/sUI/widget.c"

$(IntermediateDirectory)/sUI_widget.c$(PreprocessSuffix): ../../sys/sUI/widget.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_widget.c$(PreprocessSuffix) "../../sys/sUI/widget.c"

$(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix): ../../sys/sUI/widgetManage.c $(IntermediateDirectory)/sUI_widgetManage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/widgetManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_widgetManage.c$(DependSuffix): ../../sys/sUI/widgetManage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_widgetManage.c$(DependSuffix) -MM "../../sys/sUI/widgetManage.c"

$(IntermediateDirectory)/sUI_widgetManage.c$(PreprocessSuffix): ../../sys/sUI/widgetManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_widgetManage.c$(PreprocessSuffix) "../../sys/sUI/widgetManage.c"

$(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix): ../../sys/sUI/winManage.c $(IntermediateDirectory)/sUI_winManage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/sUI/winManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_winManage.c$(DependSuffix): ../../sys/sUI/winManage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sUI_winManage.c$(DependSuffix) -MM "../../sys/sUI/winManage.c"

$(IntermediateDirectory)/sUI_winManage.c$(PreprocessSuffix): ../../sys/sUI/winManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_winManage.c$(PreprocessSuffix) "../../sys/sUI/winManage.c"

$(IntermediateDirectory)/spi_spi.c$(ObjectSuffix): ../../sys/spi/spi.c $(IntermediateDirectory)/spi_spi.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/spi/spi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/spi_spi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/spi_spi.c$(DependSuffix): ../../sys/spi/spi.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/spi_spi.c$(ObjectSuffix) -MF$(IntermediateDirectory)/spi_spi.c$(DependSuffix) -MM "../../sys/spi/spi.c"

$(IntermediateDirectory)/spi_spi.c$(PreprocessSuffix): ../../sys/spi/spi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/spi_spi.c$(PreprocessSuffix) "../../sys/spi/spi.c"

$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix): ../../sys/streamMalloc/stream_malloc.c $(IntermediateDirectory)/streamMalloc_stream_malloc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/streamMalloc/stream_malloc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(DependSuffix): ../../sys/streamMalloc/stream_malloc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(DependSuffix) -MM "../../sys/streamMalloc/stream_malloc.c"

$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(PreprocessSuffix): ../../sys/streamMalloc/stream_malloc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/streamMalloc_stream_malloc.c$(PreprocessSuffix) "../../sys/streamMalloc/stream_malloc.c"

$(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix): ../../sys/systemSetting/systemSetting.c $(IntermediateDirectory)/systemSetting_systemSetting.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/systemSetting/systemSetting.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/systemSetting_systemSetting.c$(DependSuffix): ../../sys/systemSetting/systemSetting.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix) -MF$(IntermediateDirectory)/systemSetting_systemSetting.c$(DependSuffix) -MM "../../sys/systemSetting/systemSetting.c"

$(IntermediateDirectory)/systemSetting_systemSetting.c$(PreprocessSuffix): ../../sys/systemSetting/systemSetting.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/systemSetting_systemSetting.c$(PreprocessSuffix) "../../sys/systemSetting/systemSetting.c"

$(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix): ../../sys/tcpControl/setJSON.c $(IntermediateDirectory)/tcpControl_setJSON.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/tcpControl/setJSON.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcpControl_setJSON.c$(DependSuffix): ../../sys/tcpControl/setJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tcpControl_setJSON.c$(DependSuffix) -MM "../../sys/tcpControl/setJSON.c"

$(IntermediateDirectory)/tcpControl_setJSON.c$(PreprocessSuffix): ../../sys/tcpControl/setJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcpControl_setJSON.c$(PreprocessSuffix) "../../sys/tcpControl/setJSON.c"

$(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix): ../../sys/tcpControl/tcpControl.c $(IntermediateDirectory)/tcpControl_tcpControl.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/tcpControl/tcpControl.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcpControl_tcpControl.c$(DependSuffix): ../../sys/tcpControl/tcpControl.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tcpControl_tcpControl.c$(DependSuffix) -MM "../../sys/tcpControl/tcpControl.c"

$(IntermediateDirectory)/tcpControl_tcpControl.c$(PreprocessSuffix): ../../sys/tcpControl/tcpControl.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcpControl_tcpControl.c$(PreprocessSuffix) "../../sys/tcpControl/tcpControl.c"

$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix): ../../sys/tcpControl/tcpMessage.c $(IntermediateDirectory)/tcpControl_tcpMessage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/tcpControl/tcpMessage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcpControl_tcpMessage.c$(DependSuffix): ../../sys/tcpControl/tcpMessage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tcpControl_tcpMessage.c$(DependSuffix) -MM "../../sys/tcpControl/tcpMessage.c"

$(IntermediateDirectory)/tcpControl_tcpMessage.c$(PreprocessSuffix): ../../sys/tcpControl/tcpMessage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcpControl_tcpMessage.c$(PreprocessSuffix) "../../sys/tcpControl/tcpMessage.c"

$(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix): ../../sys/tcphlpr/pmsg.c $(IntermediateDirectory)/tcphlpr_pmsg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/tcphlpr/pmsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcphlpr_pmsg.c$(DependSuffix): ../../sys/tcphlpr/pmsg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tcphlpr_pmsg.c$(DependSuffix) -MM "../../sys/tcphlpr/pmsg.c"

$(IntermediateDirectory)/tcphlpr_pmsg.c$(PreprocessSuffix): ../../sys/tcphlpr/pmsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcphlpr_pmsg.c$(PreprocessSuffix) "../../sys/tcphlpr/pmsg.c"

$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix): ../../sys/tcphlpr/tcphlpr.c $(IntermediateDirectory)/tcphlpr_tcphlpr.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/tcphlpr/tcphlpr.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(DependSuffix): ../../sys/tcphlpr/tcphlpr.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(DependSuffix) -MM "../../sys/tcphlpr/tcphlpr.c"

$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(PreprocessSuffix): ../../sys/tcphlpr/tcphlpr.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcphlpr_tcphlpr.c$(PreprocessSuffix) "../../sys/tcphlpr/tcphlpr.c"

$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/dhcpd.c $(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/udhcp-0.9.8/dhcpd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(DependSuffix): ../../sys/udhcp-0.9.8/dhcpd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(DependSuffix) -MM "../../sys/udhcp-0.9.8/dhcpd.c"

$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/dhcpd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/dhcpd.c"

$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/leases.c $(IntermediateDirectory)/udhcp-0.9.8_leases.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/udhcp-0.9.8/leases.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(DependSuffix): ../../sys/udhcp-0.9.8/leases.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix) -MF$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(DependSuffix) -MM "../../sys/udhcp-0.9.8/leases.c"

$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/leases.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_leases.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/leases.c"

$(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/options.c $(IntermediateDirectory)/udhcp-0.9.8_options.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/udhcp-0.9.8/options.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_options.c$(DependSuffix): ../../sys/udhcp-0.9.8/options.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix) -MF$(IntermediateDirectory)/udhcp-0.9.8_options.c$(DependSuffix) -MM "../../sys/udhcp-0.9.8/options.c"

$(IntermediateDirectory)/udhcp-0.9.8_options.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/options.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_options.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/options.c"

$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/packet.c $(IntermediateDirectory)/udhcp-0.9.8_packet.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/udhcp-0.9.8/packet.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(DependSuffix): ../../sys/udhcp-0.9.8/packet.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix) -MF$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(DependSuffix) -MM "../../sys/udhcp-0.9.8/packet.c"

$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/packet.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_packet.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/packet.c"

$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/serverpacket.c $(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/udhcp-0.9.8/serverpacket.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(DependSuffix): ../../sys/udhcp-0.9.8/serverpacket.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix) -MF$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(DependSuffix) -MM "../../sys/udhcp-0.9.8/serverpacket.c"

$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/serverpacket.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/serverpacket.c"

$(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix): ../../sys/udptest/udptest.c $(IntermediateDirectory)/udptest_udptest.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/udptest/udptest.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udptest_udptest.c$(DependSuffix): ../../sys/udptest/udptest.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix) -MF$(IntermediateDirectory)/udptest_udptest.c$(DependSuffix) -MM "../../sys/udptest/udptest.c"

$(IntermediateDirectory)/udptest_udptest.c$(PreprocessSuffix): ../../sys/udptest/udptest.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udptest_udptest.c$(PreprocessSuffix) "../../sys/udptest/udptest.c"

$(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix): ../../sys/fatfs/fileManage.c $(IntermediateDirectory)/fatfs_fileManage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/fatfs/fileManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fatfs_fileManage.c$(DependSuffix): ../../sys/fatfs/fileManage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_fileManage.c$(DependSuffix) -MM "../../sys/fatfs/fileManage.c"

$(IntermediateDirectory)/fatfs_fileManage.c$(PreprocessSuffix): ../../sys/fatfs/fileManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fatfs_fileManage.c$(PreprocessSuffix) "../../sys/fatfs/fileManage.c"

$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix): ../../sys/fatfs/fileOps.c $(IntermediateDirectory)/fatfs_fileOps.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/fatfs/fileOps.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fatfs_fileOps.c$(DependSuffix): ../../sys/fatfs/fileOps.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fatfs_fileOps.c$(DependSuffix) -MM "../../sys/fatfs/fileOps.c"

$(IntermediateDirectory)/fatfs_fileOps.c$(PreprocessSuffix): ../../sys/fatfs/fileOps.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fatfs_fileOps.c$(PreprocessSuffix) "../../sys/fatfs/fileOps.c"

$(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix): ../../sys/appcmd/socket_cmd.c $(IntermediateDirectory)/appcmd_socket_cmd.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/appcmd/socket_cmd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/appcmd_socket_cmd.c$(DependSuffix): ../../sys/appcmd/socket_cmd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/appcmd_socket_cmd.c$(DependSuffix) -MM "../../sys/appcmd/socket_cmd.c"

$(IntermediateDirectory)/appcmd_socket_cmd.c$(PreprocessSuffix): ../../sys/appcmd/socket_cmd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/appcmd_socket_cmd.c$(PreprocessSuffix) "../../sys/appcmd/socket_cmd.c"

$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix): ../../sys/appcmd/socket_cmdEx.c $(IntermediateDirectory)/appcmd_socket_cmdEx.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/appcmd/socket_cmdEx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(DependSuffix): ../../sys/appcmd/socket_cmdEx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(DependSuffix) -MM "../../sys/appcmd/socket_cmdEx.c"

$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(PreprocessSuffix): ../../sys/appcmd/socket_cmdEx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/appcmd_socket_cmdEx.c$(PreprocessSuffix) "../../sys/appcmd/socket_cmdEx.c"

$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix): ../../sys/appcmd/socket_cmd_wifi.c $(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/appcmd/socket_cmd_wifi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(DependSuffix): ../../sys/appcmd/socket_cmd_wifi.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix) -MF$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(DependSuffix) -MM "../../sys/appcmd/socket_cmd_wifi.c"

$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(PreprocessSuffix): ../../sys/appcmd/socket_cmd_wifi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(PreprocessSuffix) "../../sys/appcmd/socket_cmd_wifi.c"

$(IntermediateDirectory)/printer_printer.c$(ObjectSuffix): ../../sys/printer/printer.c $(IntermediateDirectory)/printer_printer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/printer/printer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printer_printer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printer_printer.c$(DependSuffix): ../../sys/printer/printer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/printer_printer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/printer_printer.c$(DependSuffix) -MM "../../sys/printer/printer.c"

$(IntermediateDirectory)/printer_printer.c$(PreprocessSuffix): ../../sys/printer/printer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printer_printer.c$(PreprocessSuffix) "../../sys/printer/printer.c"

$(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix): ../../sys/printer/printerpic.c $(IntermediateDirectory)/printer_printerpic.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/printer/printerpic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printer_printerpic.c$(DependSuffix): ../../sys/printer/printerpic.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix) -MF$(IntermediateDirectory)/printer_printerpic.c$(DependSuffix) -MM "../../sys/printer/printerpic.c"

$(IntermediateDirectory)/printer_printerpic.c$(PreprocessSuffix): ../../sys/printer/printerpic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printer_printerpic.c$(PreprocessSuffix) "../../sys/printer/printerpic.c"

$(IntermediateDirectory)/csi_common.c$(ObjectSuffix): ../../sys/drv/csi/common.c $(IntermediateDirectory)/csi_common.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/csi/common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_common.c$(DependSuffix): ../../sys/drv/csi/common.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/csi_common.c$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_common.c$(DependSuffix) -MM "../../sys/drv/csi/common.c"

$(IntermediateDirectory)/csi_common.c$(PreprocessSuffix): ../../sys/drv/csi/common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_common.c$(PreprocessSuffix) "../../sys/drv/csi/common.c"

$(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix): ../../sys/drv/csi/csiScale0.c $(IntermediateDirectory)/csi_csiScale0.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/csi/csiScale0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_csiScale0.c$(DependSuffix): ../../sys/drv/csi/csiScale0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_csiScale0.c$(DependSuffix) -MM "../../sys/drv/csi/csiScale0.c"

$(IntermediateDirectory)/csi_csiScale0.c$(PreprocessSuffix): ../../sys/drv/csi/csiScale0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_csiScale0.c$(PreprocessSuffix) "../../sys/drv/csi/csiScale0.c"

$(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix): ../../sys/drv/csi/csiScale1.c $(IntermediateDirectory)/csi_csiScale1.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/csi/csiScale1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_csiScale1.c$(DependSuffix): ../../sys/drv/csi/csiScale1.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_csiScale1.c$(DependSuffix) -MM "../../sys/drv/csi/csiScale1.c"

$(IntermediateDirectory)/csi_csiScale1.c$(PreprocessSuffix): ../../sys/drv/csi/csiScale1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_csiScale1.c$(PreprocessSuffix) "../../sys/drv/csi/csiScale1.c"

$(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix): ../../sys/drv/csi/csiScaleuvc.c $(IntermediateDirectory)/csi_csiScaleuvc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/csi/csiScaleuvc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_csiScaleuvc.c$(DependSuffix): ../../sys/drv/csi/csiScaleuvc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_csiScaleuvc.c$(DependSuffix) -MM "../../sys/drv/csi/csiScaleuvc.c"

$(IntermediateDirectory)/csi_csiScaleuvc.c$(PreprocessSuffix): ../../sys/drv/csi/csiScaleuvc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_csiScaleuvc.c$(PreprocessSuffix) "../../sys/drv/csi/csiScaleuvc.c"

$(IntermediateDirectory)/dac_dac.c$(ObjectSuffix): ../../sys/drv/dac/dac.c $(IntermediateDirectory)/dac_dac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/dac/dac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dac_dac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dac_dac.c$(DependSuffix): ../../sys/drv/dac/dac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dac_dac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/dac_dac.c$(DependSuffix) -MM "../../sys/drv/dac/dac.c"

$(IntermediateDirectory)/dac_dac.c$(PreprocessSuffix): ../../sys/drv/dac/dac.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dac_dac.c$(PreprocessSuffix) "../../sys/drv/dac/dac.c"

$(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix): ../../sys/drv/freeRTOS/timer.c $(IntermediateDirectory)/freeRTOS_timer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/freeRTOS/timer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/freeRTOS_timer.c$(DependSuffix): ../../sys/drv/freeRTOS/timer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/freeRTOS_timer.c$(DependSuffix) -MM "../../sys/drv/freeRTOS/timer.c"

$(IntermediateDirectory)/freeRTOS_timer.c$(PreprocessSuffix): ../../sys/drv/freeRTOS/timer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/freeRTOS_timer.c$(PreprocessSuffix) "../../sys/drv/freeRTOS/timer.c"

$(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix): ../../sys/drv/i2c/i2c.c $(IntermediateDirectory)/i2c_i2c.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/i2c/i2c.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/i2c_i2c.c$(DependSuffix): ../../sys/drv/i2c/i2c.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix) -MF$(IntermediateDirectory)/i2c_i2c.c$(DependSuffix) -MM "../../sys/drv/i2c/i2c.c"

$(IntermediateDirectory)/i2c_i2c.c$(PreprocessSuffix): ../../sys/drv/i2c/i2c.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/i2c_i2c.c$(PreprocessSuffix) "../../sys/drv/i2c/i2c.c"

$(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix): ../../sys/drv/isp/hal_isp.c $(IntermediateDirectory)/isp_hal_isp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/isp/hal_isp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/isp_hal_isp.c$(DependSuffix): ../../sys/drv/isp/hal_isp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/isp_hal_isp.c$(DependSuffix) -MM "../../sys/drv/isp/hal_isp.c"

$(IntermediateDirectory)/isp_hal_isp.c$(PreprocessSuffix): ../../sys/drv/isp/hal_isp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/isp_hal_isp.c$(PreprocessSuffix) "../../sys/drv/isp/hal_isp.c"

$(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix): ../../sys/drv/lcd/lcdb.c $(IntermediateDirectory)/lcd_lcdb.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/lcd/lcdb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdb.c$(DependSuffix): ../../sys/drv/lcd/lcdb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcdb.c$(DependSuffix) -MM "../../sys/drv/lcd/lcdb.c"

$(IntermediateDirectory)/lcd_lcdb.c$(PreprocessSuffix): ../../sys/drv/lcd/lcdb.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdb.c$(PreprocessSuffix) "../../sys/drv/lcd/lcdb.c"

$(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix): ../../sys/drv/lcd/lcddev.c $(IntermediateDirectory)/lcd_lcddev.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/lcd/lcddev.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcddev.c$(DependSuffix): ../../sys/drv/lcd/lcddev.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcddev.c$(DependSuffix) -MM "../../sys/drv/lcd/lcddev.c"

$(IntermediateDirectory)/lcd_lcddev.c$(PreprocessSuffix): ../../sys/drv/lcd/lcddev.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcddev.c$(PreprocessSuffix) "../../sys/drv/lcd/lcddev.c"

$(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix): ../../sys/drv/lcd/lcdframe.c $(IntermediateDirectory)/lcd_lcdframe.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/lcd/lcdframe.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdframe.c$(DependSuffix): ../../sys/drv/lcd/lcdframe.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcdframe.c$(DependSuffix) -MM "../../sys/drv/lcd/lcdframe.c"

$(IntermediateDirectory)/lcd_lcdframe.c$(PreprocessSuffix): ../../sys/drv/lcd/lcdframe.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdframe.c$(PreprocessSuffix) "../../sys/drv/lcd/lcdframe.c"

$(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix): ../../sys/drv/lcd/lcdrotate.c $(IntermediateDirectory)/lcd_lcdrotate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/lcd/lcdrotate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdrotate.c$(DependSuffix): ../../sys/drv/lcd/lcdrotate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_lcdrotate.c$(DependSuffix) -MM "../../sys/drv/lcd/lcdrotate.c"

$(IntermediateDirectory)/lcd_lcdrotate.c$(PreprocessSuffix): ../../sys/drv/lcd/lcdrotate.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdrotate.c$(PreprocessSuffix) "../../sys/drv/lcd/lcdrotate.c"

$(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix): ../../sys/drv/lcd/picCombine.c $(IntermediateDirectory)/lcd_picCombine.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/lcd/picCombine.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_picCombine.c$(DependSuffix): ../../sys/drv/lcd/picCombine.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_picCombine.c$(DependSuffix) -MM "../../sys/drv/lcd/picCombine.c"

$(IntermediateDirectory)/lcd_picCombine.c$(PreprocessSuffix): ../../sys/drv/lcd/picCombine.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_picCombine.c$(PreprocessSuffix) "../../sys/drv/lcd/picCombine.c"

$(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix): ../../sys/drv/lcd/uiDraw.c $(IntermediateDirectory)/lcd_uiDraw.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/lcd/uiDraw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_uiDraw.c$(DependSuffix): ../../sys/drv/lcd/uiDraw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_uiDraw.c$(DependSuffix) -MM "../../sys/drv/lcd/uiDraw.c"

$(IntermediateDirectory)/lcd_uiDraw.c$(PreprocessSuffix): ../../sys/drv/lcd/uiDraw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_uiDraw.c$(PreprocessSuffix) "../../sys/drv/lcd/uiDraw.c"

$(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix): ../../sys/drv/rtc/rtc.c $(IntermediateDirectory)/rtc_rtc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/rtc/rtc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtc_rtc.c$(DependSuffix): ../../sys/drv/rtc/rtc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/rtc_rtc.c$(DependSuffix) -MM "../../sys/drv/rtc/rtc.c"

$(IntermediateDirectory)/rtc_rtc.c$(PreprocessSuffix): ../../sys/drv/rtc/rtc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtc_rtc.c$(PreprocessSuffix) "../../sys/drv/rtc/rtc.c"

$(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix): ../../sys/drv/sensor/cmos_sensor.c $(IntermediateDirectory)/sensor_cmos_sensor.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/sensor/cmos_sensor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_cmos_sensor.c$(DependSuffix): ../../sys/drv/sensor/cmos_sensor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sensor_cmos_sensor.c$(DependSuffix) -MM "../../sys/drv/sensor/cmos_sensor.c"

$(IntermediateDirectory)/sensor_cmos_sensor.c$(PreprocessSuffix): ../../sys/drv/sensor/cmos_sensor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_cmos_sensor.c$(PreprocessSuffix) "../../sys/drv/sensor/cmos_sensor.c"

$(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix): ../../sys/drv/usb_host/usensor.c $(IntermediateDirectory)/usb_host_usensor.c$(DependSuffix)
	$(CC) $(SourceSwitch) "E:/proj_code/WIFI_YG-K2T-TC/firmware/sys/drv/usb_host/usensor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_host_usensor.c$(DependSuffix): ../../sys/drv/usb_host/usensor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix) -MF$(IntermediateDirectory)/usb_host_usensor.c$(DependSuffix) -MM "../../sys/drv/usb_host/usensor.c"

$(IntermediateDirectory)/usb_host_usensor.c$(PreprocessSuffix): ../../sys/drv/usb_host/usensor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_host_usensor.c$(PreprocessSuffix) "../../sys/drv/usb_host/usensor.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


