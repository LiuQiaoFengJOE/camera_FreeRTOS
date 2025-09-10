##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-sys
ConfigurationName      :=Debug
WorkspacePath          := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build"
ProjectPath            := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\AX3292"
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
OutputFile             :=../lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)DEBUG= 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AX3292-sys.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../lib $(IncludeSwitch)../../lib/drv $(IncludeSwitch)../../sys $(IncludeSwitch)../../inc $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../arch/ $(IncludeSwitch)../../arch/AX3292/hal/inc $(IncludeSwitch)../../arch/AX3292/ax32xx/inc $(IncludeSwitch)../../sys/drv $(IncludeSwitch)../../board/AX3292/mvl8801 $(IncludeSwitch)../../demo/helloftv $(IncludeSwitch)../../demo/helloftv/res $(IncludeSwitch)../../arch/AX3292/inc/asm $(IncludeSwitch)../../sys/unifont $(IncludeSwitch)../../inc/osal 
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
	$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix) $(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix) $(IntermediateDirectory)/spi_spi.c$(ObjectSuffix) $(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix) $(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix) $(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix) $(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix) \
	$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix) $(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix) $(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix) $(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix) $(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix) $(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix) \
	$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix) $(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix) $(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix) $(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix) $(IntermediateDirectory)/printer_printer.c$(ObjectSuffix) $(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix) $(IntermediateDirectory)/unifont_ebook_font.c$(ObjectSuffix) $(IntermediateDirectory)/unifont_ebook_port.c$(ObjectSuffix) $(IntermediateDirectory)/iperf2_iperf2.c$(ObjectSuffix) $(IntermediateDirectory)/zbar_zbar_identify.c$(ObjectSuffix) \
	$(IntermediateDirectory)/btcom_btcom_user.c$(ObjectSuffix) $(IntermediateDirectory)/finsh_msh.c$(ObjectSuffix) $(IntermediateDirectory)/finsh_shell.c$(ObjectSuffix) $(IntermediateDirectory)/csi_common.c$(ObjectSuffix) $(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix) $(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix) $(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix) $(IntermediateDirectory)/dac_dac.c$(ObjectSuffix) $(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix) $(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix) \
	$(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix) $(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix) $(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix) $(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix) \
	$(IntermediateDirectory)/usb_dev_hal_usb_api.c$(ObjectSuffix) $(IntermediateDirectory)/usb_dev_hal_usb_dev_enum.c$(ObjectSuffix) $(IntermediateDirectory)/usb_dev_hal_usb_msc.c$(ObjectSuffix) $(IntermediateDirectory)/usb_dev_hal_usb_uac.c$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/usb_dev_hal_usb_uvc.c$(ObjectSuffix) $(IntermediateDirectory)/usb_dev_usb_dev.c$(ObjectSuffix) $(IntermediateDirectory)/src_error.c$(ObjectSuffix) $(IntermediateDirectory)/src_image.c$(ObjectSuffix) $(IntermediateDirectory)/src_img_scanner.c$(ObjectSuffix) $(IntermediateDirectory)/src_platform_if.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_processor.c$(ObjectSuffix) $(IntermediateDirectory)/src_refcnt.c$(ObjectSuffix) $(IntermediateDirectory)/src_scanner.c$(ObjectSuffix) $(IntermediateDirectory)/src_svg.c$(ObjectSuffix) $(IntermediateDirectory)/src_symbol.c$(ObjectSuffix) $(IntermediateDirectory)/src_zbar_decoder.c$(ObjectSuffix) $(IntermediateDirectory)/decoder_code128.c$(ObjectSuffix) $(IntermediateDirectory)/decoder_code39.c$(ObjectSuffix) $(IntermediateDirectory)/decoder_ean.c$(ObjectSuffix) $(IntermediateDirectory)/decoder_i25.c$(ObjectSuffix) \
	$(IntermediateDirectory)/decoder_pdf417.c$(ObjectSuffix) $(IntermediateDirectory)/decoder_qr_finder.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_bch15_5.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_binarize.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_isaac.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_qrdec.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_qrdectxt.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_rs.c$(ObjectSuffix) $(IntermediateDirectory)/qrcode_util.c$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/src_genaliases.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_genaliases2.c$(ObjectSuffix) $(IntermediateDirectory)/src_genflags.c$(ObjectSuffix) $(IntermediateDirectory)/src_gentranslit.c$(ObjectSuffix) $(IntermediateDirectory)/src_iconv.c$(ObjectSuffix) $(IntermediateDirectory)/src_localcharset.c$(ObjectSuffix) 

Objects4=$(IntermediateDirectory)/src_relocatable.c$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) $(Objects4) 

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
	@echo $(Objects2) >> $(ObjectsFileList)
	@echo $(Objects3) >> $(ObjectsFileList)
	@echo $(Objects4) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug"
	@echo rebuilt > "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug/AX3292-sys"

./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/auadc_auadc.c$(ObjectSuffix): ../../sys/auadc/auadc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/auadc/auadc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/auadc_auadc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/auadc_auadc.c$(PreprocessSuffix): ../../sys/auadc/auadc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/auadc_auadc.c$(PreprocessSuffix) "../../sys/auadc/auadc.c"

$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix): ../../sys/cjson/cJSON.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/cjson/cJSON.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cjson_cJSON.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cjson_cJSON.c$(PreprocessSuffix): ../../sys/cjson/cJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cjson_cJSON.c$(PreprocessSuffix) "../../sys/cjson/cJSON.c"

$(IntermediateDirectory)/config_conf_parse.c$(ObjectSuffix): ../../sys/config/conf_parse.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/config/conf_parse.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_conf_parse.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_conf_parse.c$(PreprocessSuffix): ../../sys/config/conf_parse.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_conf_parse.c$(PreprocessSuffix) "../../sys/config/conf_parse.c"

$(IntermediateDirectory)/config_conf_scan.c$(ObjectSuffix): ../../sys/config/conf_scan.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/config/conf_scan.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_conf_scan.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_conf_scan.c$(PreprocessSuffix): ../../sys/config/conf_scan.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_conf_scan.c$(PreprocessSuffix) "../../sys/config/conf_scan.c"

$(IntermediateDirectory)/config_scan.c$(ObjectSuffix): ../../sys/config/scan.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/config/scan.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_scan.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_scan.c$(PreprocessSuffix): ../../sys/config/scan.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_scan.c$(PreprocessSuffix) "../../sys/config/scan.c"

$(IntermediateDirectory)/config_scan1.c$(ObjectSuffix): ../../sys/config/scan1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/config/scan1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/config_scan1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/config_scan1.c$(PreprocessSuffix): ../../sys/config/scan1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/config_scan1.c$(PreprocessSuffix) "../../sys/config/scan1.c"

$(IntermediateDirectory)/eloop_eloop.c$(ObjectSuffix): ../../sys/eloop/eloop.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/eloop/eloop.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eloop_eloop.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eloop_eloop.c$(PreprocessSuffix): ../../sys/eloop/eloop.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eloop_eloop.c$(PreprocessSuffix) "../../sys/eloop/eloop.c"

$(IntermediateDirectory)/eloop_irqTimer.c$(ObjectSuffix): ../../sys/eloop/irqTimer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/eloop/irqTimer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eloop_irqTimer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eloop_irqTimer.c$(PreprocessSuffix): ../../sys/eloop/irqTimer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eloop_irqTimer.c$(PreprocessSuffix) "../../sys/eloop/irqTimer.c"

$(IntermediateDirectory)/eventTimer_eventTimer.c$(ObjectSuffix): ../../sys/eventTimer/eventTimer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/eventTimer/eventTimer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/eventTimer_eventTimer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/eventTimer_eventTimer.c$(PreprocessSuffix): ../../sys/eventTimer/eventTimer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/eventTimer_eventTimer.c$(PreprocessSuffix) "../../sys/eventTimer/eventTimer.c"

$(IntermediateDirectory)/fly_fly.c$(ObjectSuffix): ../../sys/fly/fly.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/fly/fly.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fly_fly.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fly_fly.c$(PreprocessSuffix): ../../sys/fly/fly.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fly_fly.c$(PreprocessSuffix) "../../sys/fly/fly.c"

$(IntermediateDirectory)/ftp_ftp-fs.c$(ObjectSuffix): ../../sys/ftp/ftp-fs.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/ftp/ftp-fs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ftp_ftp-fs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ftp_ftp-fs.c$(PreprocessSuffix): ../../sys/ftp/ftp-fs.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ftp_ftp-fs.c$(PreprocessSuffix) "../../sys/ftp/ftp-fs.c"

$(IntermediateDirectory)/ftp_ftp.c$(ObjectSuffix): ../../sys/ftp/ftp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/ftp/ftp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ftp_ftp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ftp_ftp.c$(PreprocessSuffix): ../../sys/ftp/ftp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ftp_ftp.c$(PreprocessSuffix) "../../sys/ftp/ftp.c"

$(IntermediateDirectory)/hal_watermark_ascii.c$(ObjectSuffix): ../../sys/hal_watermark/ascii.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii.c"

$(IntermediateDirectory)/hal_watermark_ascii_default.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_default.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_default.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_default.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_default.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_default.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_default.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_default.c"

$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_default_draw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_default_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_default_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_default_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_default_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num2.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num2.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num2.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num2.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num2.c"

$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num2_draw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num2_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num2_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num2_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num2_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num3.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num3.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num3.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num3.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num3.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num3.c"

$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num3_draw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num3_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num3_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num3_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num3_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num4.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num4.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num4.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num4.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num4.c"

$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num4_draw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num4_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num4_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num4_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num4_draw.c"

$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num5.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num5.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num5.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num5.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num5.c"

$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(ObjectSuffix): ../../sys/hal_watermark/ascii_num5_draw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/ascii_num5_draw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(PreprocessSuffix): ../../sys/hal_watermark/ascii_num5_draw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_ascii_num5_draw.c$(PreprocessSuffix) "../../sys/hal_watermark/ascii_num5_draw.c"

$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(ObjectSuffix): ../../sys/hal_watermark/hal_watermark.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/hal_watermark.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_hal_watermark.c$(PreprocessSuffix): ../../sys/hal_watermark/hal_watermark.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_hal_watermark.c$(PreprocessSuffix) "../../sys/hal_watermark/hal_watermark.c"

$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(ObjectSuffix): ../../sys/hal_watermark/watermark_stream.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/hal_watermark/watermark_stream.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_watermark_watermark_stream.c$(PreprocessSuffix): ../../sys/hal_watermark/watermark_stream.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_watermark_watermark_stream.c$(PreprocessSuffix) "../../sys/hal_watermark/watermark_stream.c"

$(IntermediateDirectory)/http_http.c$(ObjectSuffix): ../../sys/http/http.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/http/http.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/http_http.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/http_http.c$(PreprocessSuffix): ../../sys/http/http.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/http_http.c$(PreprocessSuffix) "../../sys/http/http.c"

$(IntermediateDirectory)/mac_fd_mac_fd.c$(ObjectSuffix): ../../sys/mac_fd/mac_fd.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/mac_fd/mac_fd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mac_fd_mac_fd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mac_fd_mac_fd.c$(PreprocessSuffix): ../../sys/mac_fd/mac_fd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mac_fd_mac_fd.c$(PreprocessSuffix) "../../sys/mac_fd/mac_fd.c"

$(IntermediateDirectory)/mediaFile_playVideo.c$(ObjectSuffix): ../../sys/mediaFile/playVideo.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/mediaFile/playVideo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_playVideo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_playVideo.c$(PreprocessSuffix): ../../sys/mediaFile/playVideo.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_playVideo.c$(PreprocessSuffix) "../../sys/mediaFile/playVideo.c"

$(IntermediateDirectory)/mediaFile_record.c$(ObjectSuffix): ../../sys/mediaFile/record.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/mediaFile/record.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_record.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_record.c$(PreprocessSuffix): ../../sys/mediaFile/record.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_record.c$(PreprocessSuffix) "../../sys/mediaFile/record.c"

$(IntermediateDirectory)/mediaFile_playNet.c$(ObjectSuffix): ../../sys/mediaFile/playNet.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/mediaFile/playNet.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_playNet.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_playNet.c$(PreprocessSuffix): ../../sys/mediaFile/playNet.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_playNet.c$(PreprocessSuffix) "../../sys/mediaFile/playNet.c"

$(IntermediateDirectory)/mediaFile_readFile.c$(ObjectSuffix): ../../sys/mediaFile/readFile.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/mediaFile/readFile.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mediaFile_readFile.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mediaFile_readFile.c$(PreprocessSuffix): ../../sys/mediaFile/readFile.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mediaFile_readFile.c$(PreprocessSuffix) "../../sys/mediaFile/readFile.c"

$(IntermediateDirectory)/printf_printf.c$(ObjectSuffix): ../../sys/printf/printf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/printf/printf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printf_printf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printf_printf.c$(PreprocessSuffix): ../../sys/printf/printf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printf_printf.c$(PreprocessSuffix) "../../sys/printf/printf.c"

$(IntermediateDirectory)/printf_vsprintf.c$(ObjectSuffix): ../../sys/printf/vsprintf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/printf/vsprintf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printf_vsprintf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printf_vsprintf.c$(PreprocessSuffix): ../../sys/printf/vsprintf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printf_vsprintf.c$(PreprocessSuffix) "../../sys/printf/vsprintf.c"

$(IntermediateDirectory)/printf_vsscanf.c$(ObjectSuffix): ../../sys/printf/vsscanf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/printf/vsscanf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printf_vsscanf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printf_vsscanf.c$(PreprocessSuffix): ../../sys/printf/vsscanf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printf_vsscanf.c$(PreprocessSuffix) "../../sys/printf/vsscanf.c"

$(IntermediateDirectory)/resfs_resfs.c$(ObjectSuffix): ../../sys/resfs/resfs.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/resfs/resfs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/resfs_resfs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/resfs_resfs.c$(PreprocessSuffix): ../../sys/resfs/resfs.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/resfs_resfs.c$(PreprocessSuffix) "../../sys/resfs/resfs.c"

$(IntermediateDirectory)/rtp-media_rtp-audio.c$(ObjectSuffix): ../../sys/rtp-media/rtp-audio.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/rtp-media/rtp-audio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtp-media_rtp-audio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtp-media_rtp-audio.c$(PreprocessSuffix): ../../sys/rtp-media/rtp-audio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtp-media_rtp-audio.c$(PreprocessSuffix) "../../sys/rtp-media/rtp-audio.c"

$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(ObjectSuffix): ../../sys/rtp-media/rtp-jpeg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/rtp-media/rtp-jpeg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(PreprocessSuffix): ../../sys/rtp-media/rtp-jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtp-media_rtp-jpeg.c$(PreprocessSuffix) "../../sys/rtp-media/rtp-jpeg.c"

$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(ObjectSuffix): ../../sys/rtp-media/rtp-user-jpeg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/rtp-media/rtp-user-jpeg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(PreprocessSuffix): ../../sys/rtp-media/rtp-user-jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtp-media_rtp-user-jpeg.c$(PreprocessSuffix) "../../sys/rtp-media/rtp-user-jpeg.c"

$(IntermediateDirectory)/sUI_button.c$(ObjectSuffix): ../../sys/sUI/button.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/button.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_button.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_button.c$(PreprocessSuffix): ../../sys/sUI/button.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_button.c$(PreprocessSuffix) "../../sys/sUI/button.c"

$(IntermediateDirectory)/sUI_dialog.c$(ObjectSuffix): ../../sys/sUI/dialog.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/dialog.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_dialog.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_dialog.c$(PreprocessSuffix): ../../sys/sUI/dialog.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_dialog.c$(PreprocessSuffix) "../../sys/sUI/dialog.c"

$(IntermediateDirectory)/sUI_frameWin.c$(ObjectSuffix): ../../sys/sUI/frameWin.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/frameWin.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_frameWin.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_frameWin.c$(PreprocessSuffix): ../../sys/sUI/frameWin.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_frameWin.c$(PreprocessSuffix) "../../sys/sUI/frameWin.c"

$(IntermediateDirectory)/sUI_imageIcon.c$(ObjectSuffix): ../../sys/sUI/imageIcon.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/imageIcon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_imageIcon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_imageIcon.c$(PreprocessSuffix): ../../sys/sUI/imageIcon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_imageIcon.c$(PreprocessSuffix) "../../sys/sUI/imageIcon.c"

$(IntermediateDirectory)/sUI_itemManage.c$(ObjectSuffix): ../../sys/sUI/itemManage.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/itemManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_itemManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_itemManage.c$(PreprocessSuffix): ../../sys/sUI/itemManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_itemManage.c$(PreprocessSuffix) "../../sys/sUI/itemManage.c"

$(IntermediateDirectory)/sUI_memPool.c$(ObjectSuffix): ../../sys/sUI/memPool.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/memPool.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_memPool.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_memPool.c$(PreprocessSuffix): ../../sys/sUI/memPool.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_memPool.c$(PreprocessSuffix) "../../sys/sUI/memPool.c"

$(IntermediateDirectory)/sUI_menuItem.c$(ObjectSuffix): ../../sys/sUI/menuItem.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/menuItem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_menuItem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_menuItem.c$(PreprocessSuffix): ../../sys/sUI/menuItem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_menuItem.c$(PreprocessSuffix) "../../sys/sUI/menuItem.c"

$(IntermediateDirectory)/sUI_menuItemEx.c$(ObjectSuffix): ../../sys/sUI/menuItemEx.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/menuItemEx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_menuItemEx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_menuItemEx.c$(PreprocessSuffix): ../../sys/sUI/menuItemEx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_menuItemEx.c$(PreprocessSuffix) "../../sys/sUI/menuItemEx.c"

$(IntermediateDirectory)/sUI_menuOption.c$(ObjectSuffix): ../../sys/sUI/menuOption.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/menuOption.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_menuOption.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_menuOption.c$(PreprocessSuffix): ../../sys/sUI/menuOption.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_menuOption.c$(PreprocessSuffix) "../../sys/sUI/menuOption.c"

$(IntermediateDirectory)/sUI_progressBar.c$(ObjectSuffix): ../../sys/sUI/progressBar.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/progressBar.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_progressBar.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_progressBar.c$(PreprocessSuffix): ../../sys/sUI/progressBar.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_progressBar.c$(PreprocessSuffix) "../../sys/sUI/progressBar.c"

$(IntermediateDirectory)/sUI_rect.c$(ObjectSuffix): ../../sys/sUI/rect.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/rect.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_rect.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_rect.c$(PreprocessSuffix): ../../sys/sUI/rect.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_rect.c$(PreprocessSuffix) "../../sys/sUI/rect.c"

$(IntermediateDirectory)/sUI_stringIcon.c$(ObjectSuffix): ../../sys/sUI/stringIcon.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/stringIcon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_stringIcon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_stringIcon.c$(PreprocessSuffix): ../../sys/sUI/stringIcon.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_stringIcon.c$(PreprocessSuffix) "../../sys/sUI/stringIcon.c"

$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix): ../../sys/sUI/tips.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/tips.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_tips.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_tips.c$(PreprocessSuffix): ../../sys/sUI/tips.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_tips.c$(PreprocessSuffix) "../../sys/sUI/tips.c"

$(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix): ../../sys/sUI/uiAlloc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/uiAlloc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_uiAlloc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_uiAlloc.c$(PreprocessSuffix): ../../sys/sUI/uiAlloc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_uiAlloc.c$(PreprocessSuffix) "../../sys/sUI/uiAlloc.c"

$(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix): ../../sys/sUI/widget.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/widget.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_widget.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_widget.c$(PreprocessSuffix): ../../sys/sUI/widget.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_widget.c$(PreprocessSuffix) "../../sys/sUI/widget.c"

$(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix): ../../sys/sUI/widgetManage.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/widgetManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_widgetManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_widgetManage.c$(PreprocessSuffix): ../../sys/sUI/widgetManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_widgetManage.c$(PreprocessSuffix) "../../sys/sUI/widgetManage.c"

$(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix): ../../sys/sUI/winManage.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/sUI/winManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sUI_winManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sUI_winManage.c$(PreprocessSuffix): ../../sys/sUI/winManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sUI_winManage.c$(PreprocessSuffix) "../../sys/sUI/winManage.c"

$(IntermediateDirectory)/spi_spi.c$(ObjectSuffix): ../../sys/spi/spi.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/spi/spi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/spi_spi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/spi_spi.c$(PreprocessSuffix): ../../sys/spi/spi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/spi_spi.c$(PreprocessSuffix) "../../sys/spi/spi.c"

$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix): ../../sys/streamMalloc/stream_malloc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/streamMalloc/stream_malloc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/streamMalloc_stream_malloc.c$(PreprocessSuffix): ../../sys/streamMalloc/stream_malloc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/streamMalloc_stream_malloc.c$(PreprocessSuffix) "../../sys/streamMalloc/stream_malloc.c"

$(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix): ../../sys/systemSetting/systemSetting.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/systemSetting/systemSetting.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/systemSetting_systemSetting.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/systemSetting_systemSetting.c$(PreprocessSuffix): ../../sys/systemSetting/systemSetting.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/systemSetting_systemSetting.c$(PreprocessSuffix) "../../sys/systemSetting/systemSetting.c"

$(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix): ../../sys/tcpControl/setJSON.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/tcpControl/setJSON.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcpControl_setJSON.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcpControl_setJSON.c$(PreprocessSuffix): ../../sys/tcpControl/setJSON.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcpControl_setJSON.c$(PreprocessSuffix) "../../sys/tcpControl/setJSON.c"

$(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix): ../../sys/tcpControl/tcpControl.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/tcpControl/tcpControl.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcpControl_tcpControl.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcpControl_tcpControl.c$(PreprocessSuffix): ../../sys/tcpControl/tcpControl.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcpControl_tcpControl.c$(PreprocessSuffix) "../../sys/tcpControl/tcpControl.c"

$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix): ../../sys/tcpControl/tcpMessage.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/tcpControl/tcpMessage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcpControl_tcpMessage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcpControl_tcpMessage.c$(PreprocessSuffix): ../../sys/tcpControl/tcpMessage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcpControl_tcpMessage.c$(PreprocessSuffix) "../../sys/tcpControl/tcpMessage.c"

$(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix): ../../sys/tcphlpr/pmsg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/tcphlpr/pmsg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcphlpr_pmsg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcphlpr_pmsg.c$(PreprocessSuffix): ../../sys/tcphlpr/pmsg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcphlpr_pmsg.c$(PreprocessSuffix) "../../sys/tcphlpr/pmsg.c"

$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix): ../../sys/tcphlpr/tcphlpr.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/tcphlpr/tcphlpr.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tcphlpr_tcphlpr.c$(PreprocessSuffix): ../../sys/tcphlpr/tcphlpr.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tcphlpr_tcphlpr.c$(PreprocessSuffix) "../../sys/tcphlpr/tcphlpr.c"

$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/dhcpd.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/udhcp-0.9.8/dhcpd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/dhcpd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_dhcpd.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/dhcpd.c"

$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/leases.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/udhcp-0.9.8/leases.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_leases.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/leases.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_leases.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/leases.c"

$(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/options.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/udhcp-0.9.8/options.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_options.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_options.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/options.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_options.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/options.c"

$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/packet.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/udhcp-0.9.8/packet.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_packet.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/packet.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_packet.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/packet.c"

$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix): ../../sys/udhcp-0.9.8/serverpacket.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/udhcp-0.9.8/serverpacket.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(PreprocessSuffix): ../../sys/udhcp-0.9.8/serverpacket.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udhcp-0.9.8_serverpacket.c$(PreprocessSuffix) "../../sys/udhcp-0.9.8/serverpacket.c"

$(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix): ../../sys/udptest/udptest.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/udptest/udptest.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/udptest_udptest.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/udptest_udptest.c$(PreprocessSuffix): ../../sys/udptest/udptest.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/udptest_udptest.c$(PreprocessSuffix) "../../sys/udptest/udptest.c"

$(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix): ../../sys/fatfs/fileManage.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/fatfs/fileManage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_fileManage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fatfs_fileManage.c$(PreprocessSuffix): ../../sys/fatfs/fileManage.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fatfs_fileManage.c$(PreprocessSuffix) "../../sys/fatfs/fileManage.c"

$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix): ../../sys/fatfs/fileOps.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/fatfs/fileOps.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fatfs_fileOps.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fatfs_fileOps.c$(PreprocessSuffix): ../../sys/fatfs/fileOps.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fatfs_fileOps.c$(PreprocessSuffix) "../../sys/fatfs/fileOps.c"

$(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix): ../../sys/appcmd/socket_cmd.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/appcmd/socket_cmd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/appcmd_socket_cmd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/appcmd_socket_cmd.c$(PreprocessSuffix): ../../sys/appcmd/socket_cmd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/appcmd_socket_cmd.c$(PreprocessSuffix) "../../sys/appcmd/socket_cmd.c"

$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix): ../../sys/appcmd/socket_cmdEx.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/appcmd/socket_cmdEx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/appcmd_socket_cmdEx.c$(PreprocessSuffix): ../../sys/appcmd/socket_cmdEx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/appcmd_socket_cmdEx.c$(PreprocessSuffix) "../../sys/appcmd/socket_cmdEx.c"

$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix): ../../sys/appcmd/socket_cmd_wifi.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/appcmd/socket_cmd_wifi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(PreprocessSuffix): ../../sys/appcmd/socket_cmd_wifi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/appcmd_socket_cmd_wifi.c$(PreprocessSuffix) "../../sys/appcmd/socket_cmd_wifi.c"

$(IntermediateDirectory)/printer_printer.c$(ObjectSuffix): ../../sys/printer/printer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/printer/printer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printer_printer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printer_printer.c$(PreprocessSuffix): ../../sys/printer/printer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printer_printer.c$(PreprocessSuffix) "../../sys/printer/printer.c"

$(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix): ../../sys/printer/printerpic.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/printer/printerpic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/printer_printerpic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/printer_printerpic.c$(PreprocessSuffix): ../../sys/printer/printerpic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/printer_printerpic.c$(PreprocessSuffix) "../../sys/printer/printerpic.c"

$(IntermediateDirectory)/unifont_ebook_font.c$(ObjectSuffix): ../../sys/unifont/ebook_font.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/unifont/ebook_font.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/unifont_ebook_font.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/unifont_ebook_font.c$(PreprocessSuffix): ../../sys/unifont/ebook_font.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/unifont_ebook_font.c$(PreprocessSuffix) "../../sys/unifont/ebook_font.c"

$(IntermediateDirectory)/unifont_ebook_port.c$(ObjectSuffix): ../../sys/unifont/ebook_port.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/unifont/ebook_port.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/unifont_ebook_port.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/unifont_ebook_port.c$(PreprocessSuffix): ../../sys/unifont/ebook_port.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/unifont_ebook_port.c$(PreprocessSuffix) "../../sys/unifont/ebook_port.c"

$(IntermediateDirectory)/iperf2_iperf2.c$(ObjectSuffix): ../../sys/iperf2/iperf2.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/iperf2/iperf2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/iperf2_iperf2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/iperf2_iperf2.c$(PreprocessSuffix): ../../sys/iperf2/iperf2.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/iperf2_iperf2.c$(PreprocessSuffix) "../../sys/iperf2/iperf2.c"

$(IntermediateDirectory)/zbar_zbar_identify.c$(ObjectSuffix): ../../sys/zbar/zbar_identify.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar_identify.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/zbar_zbar_identify.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/zbar_zbar_identify.c$(PreprocessSuffix): ../../sys/zbar/zbar_identify.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/zbar_zbar_identify.c$(PreprocessSuffix) "../../sys/zbar/zbar_identify.c"

$(IntermediateDirectory)/btcom_btcom_user.c$(ObjectSuffix): ../../sys/btcom/btcom_user.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/btcom/btcom_user.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/btcom_btcom_user.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/btcom_btcom_user.c$(PreprocessSuffix): ../../sys/btcom/btcom_user.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/btcom_btcom_user.c$(PreprocessSuffix) "../../sys/btcom/btcom_user.c"

$(IntermediateDirectory)/finsh_msh.c$(ObjectSuffix): ../../sys/finsh/msh.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/finsh/msh.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/finsh_msh.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/finsh_msh.c$(PreprocessSuffix): ../../sys/finsh/msh.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/finsh_msh.c$(PreprocessSuffix) "../../sys/finsh/msh.c"

$(IntermediateDirectory)/finsh_shell.c$(ObjectSuffix): ../../sys/finsh/shell.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/finsh/shell.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/finsh_shell.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/finsh_shell.c$(PreprocessSuffix): ../../sys/finsh/shell.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/finsh_shell.c$(PreprocessSuffix) "../../sys/finsh/shell.c"

$(IntermediateDirectory)/csi_common.c$(ObjectSuffix): ../../sys/drv/csi/common.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/csi/common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_common.c$(PreprocessSuffix): ../../sys/drv/csi/common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_common.c$(PreprocessSuffix) "../../sys/drv/csi/common.c"

$(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix): ../../sys/drv/csi/csiScale0.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/csi/csiScale0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_csiScale0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_csiScale0.c$(PreprocessSuffix): ../../sys/drv/csi/csiScale0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_csiScale0.c$(PreprocessSuffix) "../../sys/drv/csi/csiScale0.c"

$(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix): ../../sys/drv/csi/csiScale1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/csi/csiScale1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_csiScale1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_csiScale1.c$(PreprocessSuffix): ../../sys/drv/csi/csiScale1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_csiScale1.c$(PreprocessSuffix) "../../sys/drv/csi/csiScale1.c"

$(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix): ../../sys/drv/csi/csiScaleuvc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/csi/csiScaleuvc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_csiScaleuvc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_csiScaleuvc.c$(PreprocessSuffix): ../../sys/drv/csi/csiScaleuvc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_csiScaleuvc.c$(PreprocessSuffix) "../../sys/drv/csi/csiScaleuvc.c"

$(IntermediateDirectory)/dac_dac.c$(ObjectSuffix): ../../sys/drv/dac/dac.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/dac/dac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dac_dac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dac_dac.c$(PreprocessSuffix): ../../sys/drv/dac/dac.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dac_dac.c$(PreprocessSuffix) "../../sys/drv/dac/dac.c"

$(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix): ../../sys/drv/freeRTOS/timer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/freeRTOS/timer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/freeRTOS_timer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/freeRTOS_timer.c$(PreprocessSuffix): ../../sys/drv/freeRTOS/timer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/freeRTOS_timer.c$(PreprocessSuffix) "../../sys/drv/freeRTOS/timer.c"

$(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix): ../../sys/drv/i2c/i2c.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/i2c/i2c.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/i2c_i2c.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/i2c_i2c.c$(PreprocessSuffix): ../../sys/drv/i2c/i2c.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/i2c_i2c.c$(PreprocessSuffix) "../../sys/drv/i2c/i2c.c"

$(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix): ../../sys/drv/isp/hal_isp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/isp/hal_isp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/isp_hal_isp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/isp_hal_isp.c$(PreprocessSuffix): ../../sys/drv/isp/hal_isp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/isp_hal_isp.c$(PreprocessSuffix) "../../sys/drv/isp/hal_isp.c"

$(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix): ../../sys/drv/lcd/lcdb.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/lcd/lcdb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdb.c$(PreprocessSuffix): ../../sys/drv/lcd/lcdb.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdb.c$(PreprocessSuffix) "../../sys/drv/lcd/lcdb.c"

$(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix): ../../sys/drv/lcd/lcddev.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/lcd/lcddev.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcddev.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcddev.c$(PreprocessSuffix): ../../sys/drv/lcd/lcddev.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcddev.c$(PreprocessSuffix) "../../sys/drv/lcd/lcddev.c"

$(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix): ../../sys/drv/lcd/lcdframe.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/lcd/lcdframe.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdframe.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdframe.c$(PreprocessSuffix): ../../sys/drv/lcd/lcdframe.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdframe.c$(PreprocessSuffix) "../../sys/drv/lcd/lcdframe.c"

$(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix): ../../sys/drv/lcd/lcdrotate.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/lcd/lcdrotate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdrotate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdrotate.c$(PreprocessSuffix): ../../sys/drv/lcd/lcdrotate.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdrotate.c$(PreprocessSuffix) "../../sys/drv/lcd/lcdrotate.c"

$(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix): ../../sys/drv/lcd/picCombine.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/lcd/picCombine.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_picCombine.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_picCombine.c$(PreprocessSuffix): ../../sys/drv/lcd/picCombine.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_picCombine.c$(PreprocessSuffix) "../../sys/drv/lcd/picCombine.c"

$(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix): ../../sys/drv/lcd/uiDraw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/lcd/uiDraw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_uiDraw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_uiDraw.c$(PreprocessSuffix): ../../sys/drv/lcd/uiDraw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_uiDraw.c$(PreprocessSuffix) "../../sys/drv/lcd/uiDraw.c"

$(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix): ../../sys/drv/rtc/rtc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/rtc/rtc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtc_rtc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtc_rtc.c$(PreprocessSuffix): ../../sys/drv/rtc/rtc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtc_rtc.c$(PreprocessSuffix) "../../sys/drv/rtc/rtc.c"

$(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix): ../../sys/drv/sensor/cmos_sensor.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/sensor/cmos_sensor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sensor_cmos_sensor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sensor_cmos_sensor.c$(PreprocessSuffix): ../../sys/drv/sensor/cmos_sensor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sensor_cmos_sensor.c$(PreprocessSuffix) "../../sys/drv/sensor/cmos_sensor.c"

$(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix): ../../sys/drv/usb_host/usensor.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_host/usensor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_host_usensor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_host_usensor.c$(PreprocessSuffix): ../../sys/drv/usb_host/usensor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_host_usensor.c$(PreprocessSuffix) "../../sys/drv/usb_host/usensor.c"

$(IntermediateDirectory)/usb_dev_hal_usb_api.c$(ObjectSuffix): ../../sys/drv/usb_dev/hal_usb_api.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_dev/hal_usb_api.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_dev_hal_usb_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_dev_hal_usb_api.c$(PreprocessSuffix): ../../sys/drv/usb_dev/hal_usb_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_dev_hal_usb_api.c$(PreprocessSuffix) "../../sys/drv/usb_dev/hal_usb_api.c"

$(IntermediateDirectory)/usb_dev_hal_usb_dev_enum.c$(ObjectSuffix): ../../sys/drv/usb_dev/hal_usb_dev_enum.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_dev/hal_usb_dev_enum.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_dev_hal_usb_dev_enum.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_dev_hal_usb_dev_enum.c$(PreprocessSuffix): ../../sys/drv/usb_dev/hal_usb_dev_enum.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_dev_hal_usb_dev_enum.c$(PreprocessSuffix) "../../sys/drv/usb_dev/hal_usb_dev_enum.c"

$(IntermediateDirectory)/usb_dev_hal_usb_msc.c$(ObjectSuffix): ../../sys/drv/usb_dev/hal_usb_msc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_dev/hal_usb_msc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_dev_hal_usb_msc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_dev_hal_usb_msc.c$(PreprocessSuffix): ../../sys/drv/usb_dev/hal_usb_msc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_dev_hal_usb_msc.c$(PreprocessSuffix) "../../sys/drv/usb_dev/hal_usb_msc.c"

$(IntermediateDirectory)/usb_dev_hal_usb_uac.c$(ObjectSuffix): ../../sys/drv/usb_dev/hal_usb_uac.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_dev/hal_usb_uac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_dev_hal_usb_uac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_dev_hal_usb_uac.c$(PreprocessSuffix): ../../sys/drv/usb_dev/hal_usb_uac.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_dev_hal_usb_uac.c$(PreprocessSuffix) "../../sys/drv/usb_dev/hal_usb_uac.c"

$(IntermediateDirectory)/usb_dev_hal_usb_uvc.c$(ObjectSuffix): ../../sys/drv/usb_dev/hal_usb_uvc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_dev/hal_usb_uvc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_dev_hal_usb_uvc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_dev_hal_usb_uvc.c$(PreprocessSuffix): ../../sys/drv/usb_dev/hal_usb_uvc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_dev_hal_usb_uvc.c$(PreprocessSuffix) "../../sys/drv/usb_dev/hal_usb_uvc.c"

$(IntermediateDirectory)/usb_dev_usb_dev.c$(ObjectSuffix): ../../sys/drv/usb_dev/usb_dev.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/drv/usb_dev/usb_dev.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_dev_usb_dev.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_dev_usb_dev.c$(PreprocessSuffix): ../../sys/drv/usb_dev/usb_dev.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_dev_usb_dev.c$(PreprocessSuffix) "../../sys/drv/usb_dev/usb_dev.c"

$(IntermediateDirectory)/src_error.c$(ObjectSuffix): ../../sys/zbar/zbar/src/error.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/error.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_error.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_error.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/error.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_error.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/error.c"

$(IntermediateDirectory)/src_image.c$(ObjectSuffix): ../../sys/zbar/zbar/src/image.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/image.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_image.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_image.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/image.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_image.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/image.c"

$(IntermediateDirectory)/src_img_scanner.c$(ObjectSuffix): ../../sys/zbar/zbar/src/img_scanner.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/img_scanner.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_img_scanner.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_img_scanner.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/img_scanner.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_img_scanner.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/img_scanner.c"

$(IntermediateDirectory)/src_platform_if.c$(ObjectSuffix): ../../sys/zbar/zbar/src/platform_if.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/platform_if.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_platform_if.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_platform_if.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/platform_if.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_platform_if.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/platform_if.c"

$(IntermediateDirectory)/src_processor.c$(ObjectSuffix): ../../sys/zbar/zbar/src/processor.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/processor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_processor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_processor.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/processor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_processor.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/processor.c"

$(IntermediateDirectory)/src_refcnt.c$(ObjectSuffix): ../../sys/zbar/zbar/src/refcnt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/refcnt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_refcnt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_refcnt.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/refcnt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_refcnt.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/refcnt.c"

$(IntermediateDirectory)/src_scanner.c$(ObjectSuffix): ../../sys/zbar/zbar/src/scanner.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/scanner.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_scanner.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_scanner.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/scanner.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_scanner.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/scanner.c"

$(IntermediateDirectory)/src_svg.c$(ObjectSuffix): ../../sys/zbar/zbar/src/svg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/svg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_svg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_svg.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/svg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_svg.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/svg.c"

$(IntermediateDirectory)/src_symbol.c$(ObjectSuffix): ../../sys/zbar/zbar/src/symbol.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/symbol.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_symbol.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_symbol.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/symbol.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_symbol.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/symbol.c"

$(IntermediateDirectory)/src_zbar_decoder.c$(ObjectSuffix): ../../sys/zbar/zbar/src/zbar_decoder.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/zbar_decoder.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_zbar_decoder.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_zbar_decoder.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/zbar_decoder.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_zbar_decoder.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/zbar_decoder.c"

$(IntermediateDirectory)/decoder_code128.c$(ObjectSuffix): ../../sys/zbar/zbar/src/decoder/code128.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/decoder/code128.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/decoder_code128.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/decoder_code128.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/decoder/code128.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/decoder_code128.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/decoder/code128.c"

$(IntermediateDirectory)/decoder_code39.c$(ObjectSuffix): ../../sys/zbar/zbar/src/decoder/code39.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/decoder/code39.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/decoder_code39.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/decoder_code39.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/decoder/code39.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/decoder_code39.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/decoder/code39.c"

$(IntermediateDirectory)/decoder_ean.c$(ObjectSuffix): ../../sys/zbar/zbar/src/decoder/ean.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/decoder/ean.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/decoder_ean.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/decoder_ean.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/decoder/ean.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/decoder_ean.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/decoder/ean.c"

$(IntermediateDirectory)/decoder_i25.c$(ObjectSuffix): ../../sys/zbar/zbar/src/decoder/i25.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/decoder/i25.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/decoder_i25.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/decoder_i25.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/decoder/i25.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/decoder_i25.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/decoder/i25.c"

$(IntermediateDirectory)/decoder_pdf417.c$(ObjectSuffix): ../../sys/zbar/zbar/src/decoder/pdf417.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/decoder/pdf417.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/decoder_pdf417.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/decoder_pdf417.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/decoder/pdf417.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/decoder_pdf417.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/decoder/pdf417.c"

$(IntermediateDirectory)/decoder_qr_finder.c$(ObjectSuffix): ../../sys/zbar/zbar/src/decoder/qr_finder.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/decoder/qr_finder.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/decoder_qr_finder.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/decoder_qr_finder.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/decoder/qr_finder.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/decoder_qr_finder.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/decoder/qr_finder.c"

$(IntermediateDirectory)/qrcode_bch15_5.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/bch15_5.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/bch15_5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_bch15_5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_bch15_5.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/bch15_5.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_bch15_5.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/bch15_5.c"

$(IntermediateDirectory)/qrcode_binarize.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/binarize.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/binarize.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_binarize.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_binarize.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/binarize.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_binarize.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/binarize.c"

$(IntermediateDirectory)/qrcode_isaac.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/isaac.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/isaac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_isaac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_isaac.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/isaac.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_isaac.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/isaac.c"

$(IntermediateDirectory)/qrcode_qrdec.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/qrdec.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/qrdec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_qrdec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_qrdec.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/qrdec.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_qrdec.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/qrdec.c"

$(IntermediateDirectory)/qrcode_qrdectxt.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/qrdectxt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/qrdectxt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_qrdectxt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_qrdectxt.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/qrdectxt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_qrdectxt.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/qrdectxt.c"

$(IntermediateDirectory)/qrcode_rs.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/rs.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/rs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_rs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_rs.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/rs.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_rs.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/rs.c"

$(IntermediateDirectory)/qrcode_util.c$(ObjectSuffix): ../../sys/zbar/zbar/src/qrcode/util.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/src/qrcode/util.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/qrcode_util.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/qrcode_util.c$(PreprocessSuffix): ../../sys/zbar/zbar/src/qrcode/util.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/qrcode_util.c$(PreprocessSuffix) "../../sys/zbar/zbar/src/qrcode/util.c"

$(IntermediateDirectory)/src_genaliases.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/genaliases.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/genaliases.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_genaliases.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_genaliases.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/genaliases.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_genaliases.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/genaliases.c"

$(IntermediateDirectory)/src_genaliases2.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/genaliases2.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/genaliases2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_genaliases2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_genaliases2.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/genaliases2.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_genaliases2.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/genaliases2.c"

$(IntermediateDirectory)/src_genflags.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/genflags.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/genflags.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_genflags.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_genflags.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/genflags.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_genflags.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/genflags.c"

$(IntermediateDirectory)/src_gentranslit.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/gentranslit.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/gentranslit.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gentranslit.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gentranslit.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/gentranslit.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gentranslit.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/gentranslit.c"

$(IntermediateDirectory)/src_iconv.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/iconv.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/iconv.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_iconv.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_iconv.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/iconv.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_iconv.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/iconv.c"

$(IntermediateDirectory)/src_localcharset.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/localcharset.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/localcharset.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_localcharset.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_localcharset.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/localcharset.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_localcharset.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/localcharset.c"

$(IntermediateDirectory)/src_relocatable.c$(ObjectSuffix): ../../sys/zbar/zbar/iconv/src/relocatable.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/sys/zbar/zbar/iconv/src/relocatable.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_relocatable.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_relocatable.c$(PreprocessSuffix): ../../sys/zbar/zbar/iconv/src/relocatable.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_relocatable.c$(PreprocessSuffix) "../../sys/zbar/zbar/iconv/src/relocatable.c"

##
## Clean
##
clean:
	$(RM) -r ./Debug/


