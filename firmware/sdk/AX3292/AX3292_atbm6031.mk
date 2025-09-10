##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292_atbm6031
ConfigurationName      :=Debug
WorkspacePath          := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build"
ProjectPath            := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\AX3292"
IntermediateDirectory  :=Debug
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
OutputFile             :=../lib_atbm.a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AX3292_atbm6031.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../ $(IncludeSwitch)../../lib $(IncludeSwitch)../../lib/drv $(IncludeSwitch)./../../inc/osal $(IncludeSwitch)./../../net/atbm/linux $(IncludeSwitch)./../../net/atbm/include $(IncludeSwitch)./../../net/atbm/api $(IncludeSwitch)./../../net/atbm/hal/include $(IncludeSwitch)./../../net/atbm/hal/sdio $(IncludeSwitch)./../../net/atbm/api/sdio $(IncludeSwitch)./../../net/atbm/net/include $(IncludeSwitch)./../../net/atbm/net/include/proto $(IncludeSwitch)./../../net/atbm/net/wpa/crypto/include $(IncludeSwitch)./../../net/atbm/net/wpa/wps/include $(IncludeSwitch)./../../net/atbm/net/wpa/sae/include $(IncludeSwitch)./../../net/atbm/os/jrx_rtos/include $(IncludeSwitch)./../../net/atbm/os/include $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../os/freeRTOS $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../arch/AX3292/inc $(IncludeSwitch)../../inc $(IncludeSwitch)../../inc/net $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/2.0.2/include/lwip $(IncludeSwitch)../../board/AX3292/mvl8801 $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/common $(IncludeSwitch)../../sys/drv/mmc/hal $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../arch/AX3292/inc $(IncludeSwitch)../../inc $(IncludeSwitch)../../inc/net $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../sys/drv $(IncludeSwitch)../../board/AX3292/mvl8801 $(IncludeSwitch)../../os/freeRTOS/api/inc 
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
CXXFLAGS :=  -g -Wall -Os $(Preprocessors)
CFLAGS   :=  -g -Os -Wall -fshort-wchar -Wno-pointer-sign -Wno-format -Wno-array-bounds -Wno-strict-aliasing -ffunction-sections -fdata-sections $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.7.1/bin/or1k-elf-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix) \
	$(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix) $(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_phy_reg.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_thread.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_timer.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_ble.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_ble_driver_api.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_special.c$(ObjectSuffix) \
	$(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_agg.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_bridge.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_connect.c$(ObjectSuffix) \
	$(IntermediateDirectory)/jrx_rtos_atbm_os_api.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_atomic.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_mem.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_mutex.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_sdio.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_skbuf.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_spinlock.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_thread.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_timer.c$(ObjectSuffix) $(IntermediateDirectory)/jrx_rtos_atbm_os_msgQ.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix) $(IntermediateDirectory)/compress_compress.c$(ObjectSuffix) $(IntermediateDirectory)/compress_pq.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix) \
	$(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpa_debug.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpa_event.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpabuf.c$(ObjectSuffix) $(IntermediateDirectory)/crypto_aes-cbc.c$(ObjectSuffix) $(IntermediateDirectory)/crypto_base64.c$(ObjectSuffix) $(IntermediateDirectory)/crypto_bignum.c$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/crypto_crypto_internal-modexp.c$(ObjectSuffix) $(IntermediateDirectory)/crypto_dh_group5.c$(ObjectSuffix) \
	$(IntermediateDirectory)/crypto_dh_groups.c$(ObjectSuffix) $(IntermediateDirectory)/crypto_sha256.c$(ObjectSuffix) $(IntermediateDirectory)/crypto_sha256-internal.c$(ObjectSuffix) $(IntermediateDirectory)/sae_crypto_mbedlts.c$(ObjectSuffix) $(IntermediateDirectory)/sae_dragonfly.c$(ObjectSuffix) $(IntermediateDirectory)/sae_ecp.c$(ObjectSuffix) $(IntermediateDirectory)/sae_ecp_curves.c$(ObjectSuffix) $(IntermediateDirectory)/sae_pmksa_cache.c$(ObjectSuffix) $(IntermediateDirectory)/sae_sae.c$(ObjectSuffix) $(IntermediateDirectory)/wps_eap_common.c$(ObjectSuffix) \
	$(IntermediateDirectory)/wps_eap_register.c$(ObjectSuffix) $(IntermediateDirectory)/wps_eap_server_wsc.c$(ObjectSuffix) $(IntermediateDirectory)/wps_ieee802_11_common.c$(ObjectSuffix) $(IntermediateDirectory)/wps_uuid.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_attr_build.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_attr_parse.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_attr_process.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_common.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_dev_attr.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_enrollee.c$(ObjectSuffix) \
	$(IntermediateDirectory)/wps_wps_hostapd.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_main.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_registrar.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_sta.c$(ObjectSuffix) $(IntermediateDirectory)/wps_wps_validate.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug"
	@echo rebuilt > "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug/AX3292_atbm6031"

Debug:
	@$(MakeDirCommand) "Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix): ../../net/atbm/api/atbm_api.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/api/atbm_api.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_atbm_api.c$(PreprocessSuffix): ../../net/atbm/api/atbm_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_atbm_api.c$(PreprocessSuffix) "../../net/atbm/api/atbm_api.c"

$(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix): ../../net/atbm/hal/atbm_ap.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_ap.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_ap.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_ap.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_ap.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_ap.c"

$(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix): ../../net/atbm/hal/atbm_config.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_config.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_config.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_config.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_config.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_config.c"

$(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix): ../../net/atbm/hal/atbm_etf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_etf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_etf.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_etf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_etf.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_etf.c"

$(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix): ../../net/atbm/hal/atbm_init.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_init.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_init.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_init.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_init.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_init.c"

$(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix): ../../net/atbm/hal/atbm_main.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_main.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_main.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_main.c"

$(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix): ../../net/atbm/hal/atbm_queue.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_queue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_queue.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_queue.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_queue.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_queue.c"

$(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix): ../../net/atbm/hal/atbm_skbuf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_skbuf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_skbuf.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_skbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_skbuf.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_skbuf.c"

$(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix): ../../net/atbm/hal/atbm_sta.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_sta.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_sta.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_sta.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_sta.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_sta.c"

$(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix): ../../net/atbm/hal/atbm_task.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_task.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_task.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_task.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_task.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_task.c"

$(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix): ../../net/atbm/hal/atbm_txrx.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_txrx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_txrx.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_txrx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_txrx.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_txrx.c"

$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix): ../../net/atbm/hal/atbm_wifi_driver_api.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_wifi_driver_api.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_wifi_driver_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_wifi_driver_api.c"

$(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix): ../../net/atbm/hal/atbm_wsm.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_wsm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_wsm.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_wsm.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_wsm.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_wsm.c"

$(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix): ../../net/atbm/hal/smartconfig.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/smartconfig.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_smartconfig.c$(PreprocessSuffix): ../../net/atbm/hal/smartconfig.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_smartconfig.c$(PreprocessSuffix) "../../net/atbm/hal/smartconfig.c"

$(IntermediateDirectory)/hal_atbm_phy_reg.c$(ObjectSuffix): ../../net/atbm/hal/atbm_phy_reg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_phy_reg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_phy_reg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_phy_reg.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_phy_reg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_phy_reg.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_phy_reg.c"

$(IntermediateDirectory)/hal_atbm_thread.c$(ObjectSuffix): ../../net/atbm/hal/atbm_thread.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_thread.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_thread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_thread.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_thread.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_thread.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_thread.c"

$(IntermediateDirectory)/hal_atbm_timer.c$(ObjectSuffix): ../../net/atbm/hal/atbm_timer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_timer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_timer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_timer.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_timer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_timer.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_timer.c"

$(IntermediateDirectory)/hal_atbm_ble.c$(ObjectSuffix): ../../net/atbm/hal/atbm_ble.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_ble.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_ble.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_ble.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_ble.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_ble.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_ble.c"

$(IntermediateDirectory)/hal_atbm_ble_driver_api.c$(ObjectSuffix): ../../net/atbm/hal/atbm_ble_driver_api.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_ble_driver_api.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_ble_driver_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_ble_driver_api.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_ble_driver_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_ble_driver_api.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_ble_driver_api.c"

$(IntermediateDirectory)/hal_atbm_special.c$(ObjectSuffix): ../../net/atbm/hal/atbm_special.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/atbm_special.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_special.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_special.c$(PreprocessSuffix): ../../net/atbm/hal/atbm_special.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_special.c$(PreprocessSuffix) "../../net/atbm/hal/atbm_special.c"

$(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix): ../../net/atbm/net/atbm_key.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_key.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_key.c$(PreprocessSuffix): ../../net/atbm/net/atbm_key.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_key.c$(PreprocessSuffix) "../../net/atbm/net/atbm_key.c"

$(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix): ../../net/atbm/net/atbm_mgmt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_mgmt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_mgmt.c$(PreprocessSuffix): ../../net/atbm/net/atbm_mgmt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_mgmt.c$(PreprocessSuffix) "../../net/atbm/net/atbm_mgmt.c"

$(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix): ../../net/atbm/net/atbm_ratectrl.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_ratectrl.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_ratectrl.c$(PreprocessSuffix): ../../net/atbm/net/atbm_ratectrl.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_ratectrl.c$(PreprocessSuffix) "../../net/atbm/net/atbm_ratectrl.c"

$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix): ../../net/atbm/net/atbm_rc80211_minstrel.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_rc80211_minstrel.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(PreprocessSuffix): ../../net/atbm/net/atbm_rc80211_minstrel.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(PreprocessSuffix) "../../net/atbm/net/atbm_rc80211_minstrel.c"

$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix): ../../net/atbm/net/atbm_rc80211_minstrel_ht.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_rc80211_minstrel_ht.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(PreprocessSuffix): ../../net/atbm/net/atbm_rc80211_minstrel_ht.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(PreprocessSuffix) "../../net/atbm/net/atbm_rc80211_minstrel_ht.c"

$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix): ../../net/atbm/net/atbm_rc80211_pid_algo.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_rc80211_pid_algo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(PreprocessSuffix): ../../net/atbm/net/atbm_rc80211_pid_algo.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(PreprocessSuffix) "../../net/atbm/net/atbm_rc80211_pid_algo.c"

$(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix): ../../net/atbm/net/atbm_util.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_util.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_util.c$(PreprocessSuffix): ../../net/atbm/net/atbm_util.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_util.c$(PreprocessSuffix) "../../net/atbm/net/atbm_util.c"

$(IntermediateDirectory)/net_atbm_agg.c$(ObjectSuffix): ../../net/atbm/net/atbm_agg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_agg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_agg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_agg.c$(PreprocessSuffix): ../../net/atbm/net/atbm_agg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_agg.c$(PreprocessSuffix) "../../net/atbm/net/atbm_agg.c"

$(IntermediateDirectory)/net_atbm_bridge.c$(ObjectSuffix): ../../net/atbm/net/atbm_bridge.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_bridge.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_bridge.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_bridge.c$(PreprocessSuffix): ../../net/atbm/net/atbm_bridge.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_bridge.c$(PreprocessSuffix) "../../net/atbm/net/atbm_bridge.c"

$(IntermediateDirectory)/net_atbm_connect.c$(ObjectSuffix): ../../net/atbm/net/atbm_connect.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/atbm_connect.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_connect.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_connect.c$(PreprocessSuffix): ../../net/atbm/net/atbm_connect.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_connect.c$(PreprocessSuffix) "../../net/atbm/net/atbm_connect.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_api.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_api.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_api.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_api.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_api.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_api.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_atomic.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_atomic.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_atomic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_atomic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_atomic.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_atomic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_atomic.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_atomic.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_mem.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_mem.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_mem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_mem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_mem.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_mem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_mem.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_mem.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_mutex.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_mutex.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_mutex.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_mutex.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_mutex.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_mutex.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_mutex.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_mutex.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_sdio.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_sdio.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_sdio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_sdio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_sdio.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_sdio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_sdio.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_sdio.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_skbuf.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_skbuf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_skbuf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_skbuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_skbuf.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_skbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_skbuf.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_skbuf.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_spinlock.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_spinlock.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_spinlock.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_spinlock.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_spinlock.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_spinlock.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_spinlock.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_spinlock.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_thread.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_thread.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_thread.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_thread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_thread.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_thread.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_thread.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_thread.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_timer.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_timer.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_timer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_timer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_timer.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_timer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_timer.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_timer.c"

$(IntermediateDirectory)/jrx_rtos_atbm_os_msgQ.c$(ObjectSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_msgQ.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/os/jrx_rtos/atbm_os_msgQ.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jrx_rtos_atbm_os_msgQ.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jrx_rtos_atbm_os_msgQ.c$(PreprocessSuffix): ../../net/atbm/os/jrx_rtos/atbm_os_msgQ.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jrx_rtos_atbm_os_msgQ.c$(PreprocessSuffix) "../../net/atbm/os/jrx_rtos/atbm_os_msgQ.c"

$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix): ../../net/atbm/hal/sdio/atbm_dcxo_dpll.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/sdio/atbm_dcxo_dpll.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(PreprocessSuffix): ../../net/atbm/hal/sdio/atbm_dcxo_dpll.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(PreprocessSuffix) "../../net/atbm/hal/sdio/atbm_dcxo_dpll.c"

$(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix): ../../net/atbm/hal/sdio/atbm_sdio.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/sdio/atbm_sdio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio.c$(PreprocessSuffix): ../../net/atbm/hal/sdio/atbm_sdio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio.c$(PreprocessSuffix) "../../net/atbm/hal/sdio/atbm_sdio.c"

$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix): ../../net/atbm/hal/sdio/atbm_sdio_bh.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/sdio/atbm_sdio_bh.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(PreprocessSuffix): ../../net/atbm/hal/sdio/atbm_sdio_bh.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(PreprocessSuffix) "../../net/atbm/hal/sdio/atbm_sdio_bh.c"

$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix): ../../net/atbm/hal/sdio/atbm_sdio_fwio.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/sdio/atbm_sdio_fwio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(PreprocessSuffix): ../../net/atbm/hal/sdio/atbm_sdio_fwio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(PreprocessSuffix) "../../net/atbm/hal/sdio/atbm_sdio_fwio.c"

$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix): ../../net/atbm/hal/sdio/atbm_sdio_hwio.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/sdio/atbm_sdio_hwio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(PreprocessSuffix): ../../net/atbm/hal/sdio/atbm_sdio_hwio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(PreprocessSuffix) "../../net/atbm/hal/sdio/atbm_sdio_hwio.c"

$(IntermediateDirectory)/compress_compress.c$(ObjectSuffix): ../../net/atbm/hal/compress/compress.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/compress/compress.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/compress_compress.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/compress_compress.c$(PreprocessSuffix): ../../net/atbm/hal/compress/compress.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/compress_compress.c$(PreprocessSuffix) "../../net/atbm/hal/compress/compress.c"

$(IntermediateDirectory)/compress_pq.c$(ObjectSuffix): ../../net/atbm/hal/compress/pq.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/hal/compress/pq.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/compress_pq.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/compress_pq.c$(PreprocessSuffix): ../../net/atbm/hal/compress/pq.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/compress_pq.c$(PreprocessSuffix) "../../net/atbm/hal/compress/pq.c"

$(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix): ../../net/atbm/net/wpa/aes_core.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/aes_core.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_aes_core.c$(PreprocessSuffix): ../../net/atbm/net/wpa/aes_core.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_aes_core.c$(PreprocessSuffix) "../../net/atbm/net/wpa/aes_core.c"

$(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix): ../../net/atbm/net/wpa/hostapd_main.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/hostapd_main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_hostapd_main.c$(PreprocessSuffix): ../../net/atbm/net/wpa/hostapd_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_hostapd_main.c$(PreprocessSuffix) "../../net/atbm/net/wpa/hostapd_main.c"

$(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix): ../../net/atbm/net/wpa/sha1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sha1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_sha1.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sha1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_sha1.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sha1.c"

$(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix): ../../net/atbm/net/wpa/wpa_common.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wpa_common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_common.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wpa_common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_common.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wpa_common.c"

$(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix): ../../net/atbm/net/wpa/wpa_main.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wpa_main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_main.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wpa_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_main.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wpa_main.c"

$(IntermediateDirectory)/wpa_wpa_debug.c$(ObjectSuffix): ../../net/atbm/net/wpa/wpa_debug.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wpa_debug.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_debug.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_debug.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wpa_debug.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_debug.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wpa_debug.c"

$(IntermediateDirectory)/wpa_wpa_event.c$(ObjectSuffix): ../../net/atbm/net/wpa/wpa_event.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wpa_event.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_event.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_event.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wpa_event.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_event.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wpa_event.c"

$(IntermediateDirectory)/wpa_wpabuf.c$(ObjectSuffix): ../../net/atbm/net/wpa/wpabuf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wpabuf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpabuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpabuf.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wpabuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpabuf.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wpabuf.c"

$(IntermediateDirectory)/crypto_aes-cbc.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/aes-cbc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/aes-cbc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_aes-cbc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_aes-cbc.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/aes-cbc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_aes-cbc.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/aes-cbc.c"

$(IntermediateDirectory)/crypto_base64.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/base64.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/base64.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_base64.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_base64.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/base64.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_base64.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/base64.c"

$(IntermediateDirectory)/crypto_bignum.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/bignum.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/bignum.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_bignum.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_bignum.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/bignum.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_bignum.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/bignum.c"

$(IntermediateDirectory)/crypto_crypto_internal-modexp.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/crypto_internal-modexp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/crypto_internal-modexp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_crypto_internal-modexp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_crypto_internal-modexp.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/crypto_internal-modexp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_crypto_internal-modexp.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/crypto_internal-modexp.c"

$(IntermediateDirectory)/crypto_dh_group5.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/dh_group5.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/dh_group5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_dh_group5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_dh_group5.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/dh_group5.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_dh_group5.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/dh_group5.c"

$(IntermediateDirectory)/crypto_dh_groups.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/dh_groups.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/dh_groups.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_dh_groups.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_dh_groups.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/dh_groups.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_dh_groups.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/dh_groups.c"

$(IntermediateDirectory)/crypto_sha256.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/sha256.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/sha256.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_sha256.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_sha256.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/sha256.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_sha256.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/sha256.c"

$(IntermediateDirectory)/crypto_sha256-internal.c$(ObjectSuffix): ../../net/atbm/net/wpa/crypto/sha256-internal.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/crypto/sha256-internal.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypto_sha256-internal.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypto_sha256-internal.c$(PreprocessSuffix): ../../net/atbm/net/wpa/crypto/sha256-internal.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypto_sha256-internal.c$(PreprocessSuffix) "../../net/atbm/net/wpa/crypto/sha256-internal.c"

$(IntermediateDirectory)/sae_crypto_mbedlts.c$(ObjectSuffix): ../../net/atbm/net/wpa/sae/crypto_mbedlts.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sae/crypto_mbedlts.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sae_crypto_mbedlts.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sae_crypto_mbedlts.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sae/crypto_mbedlts.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sae_crypto_mbedlts.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sae/crypto_mbedlts.c"

$(IntermediateDirectory)/sae_dragonfly.c$(ObjectSuffix): ../../net/atbm/net/wpa/sae/dragonfly.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sae/dragonfly.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sae_dragonfly.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sae_dragonfly.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sae/dragonfly.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sae_dragonfly.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sae/dragonfly.c"

$(IntermediateDirectory)/sae_ecp.c$(ObjectSuffix): ../../net/atbm/net/wpa/sae/ecp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sae/ecp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sae_ecp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sae_ecp.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sae/ecp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sae_ecp.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sae/ecp.c"

$(IntermediateDirectory)/sae_ecp_curves.c$(ObjectSuffix): ../../net/atbm/net/wpa/sae/ecp_curves.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sae/ecp_curves.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sae_ecp_curves.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sae_ecp_curves.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sae/ecp_curves.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sae_ecp_curves.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sae/ecp_curves.c"

$(IntermediateDirectory)/sae_pmksa_cache.c$(ObjectSuffix): ../../net/atbm/net/wpa/sae/pmksa_cache.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sae/pmksa_cache.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sae_pmksa_cache.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sae_pmksa_cache.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sae/pmksa_cache.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sae_pmksa_cache.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sae/pmksa_cache.c"

$(IntermediateDirectory)/sae_sae.c$(ObjectSuffix): ../../net/atbm/net/wpa/sae/sae.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/sae/sae.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sae_sae.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sae_sae.c$(PreprocessSuffix): ../../net/atbm/net/wpa/sae/sae.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sae_sae.c$(PreprocessSuffix) "../../net/atbm/net/wpa/sae/sae.c"

$(IntermediateDirectory)/wps_eap_common.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/eap_common.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/eap_common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_eap_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_eap_common.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/eap_common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_eap_common.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/eap_common.c"

$(IntermediateDirectory)/wps_eap_register.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/eap_register.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/eap_register.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_eap_register.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_eap_register.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/eap_register.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_eap_register.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/eap_register.c"

$(IntermediateDirectory)/wps_eap_server_wsc.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/eap_server_wsc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/eap_server_wsc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_eap_server_wsc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_eap_server_wsc.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/eap_server_wsc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_eap_server_wsc.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/eap_server_wsc.c"

$(IntermediateDirectory)/wps_ieee802_11_common.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/ieee802_11_common.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/ieee802_11_common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_ieee802_11_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_ieee802_11_common.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/ieee802_11_common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_ieee802_11_common.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/ieee802_11_common.c"

$(IntermediateDirectory)/wps_uuid.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/uuid.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/uuid.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_uuid.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_uuid.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/uuid.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_uuid.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/uuid.c"

$(IntermediateDirectory)/wps_wps_attr_build.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_attr_build.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_attr_build.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_attr_build.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_attr_build.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_attr_build.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_attr_build.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_attr_build.c"

$(IntermediateDirectory)/wps_wps_attr_parse.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_attr_parse.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_attr_parse.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_attr_parse.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_attr_parse.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_attr_parse.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_attr_parse.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_attr_parse.c"

$(IntermediateDirectory)/wps_wps_attr_process.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_attr_process.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_attr_process.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_attr_process.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_attr_process.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_attr_process.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_attr_process.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_attr_process.c"

$(IntermediateDirectory)/wps_wps_common.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_common.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_common.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_common.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_common.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_common.c"

$(IntermediateDirectory)/wps_wps_dev_attr.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_dev_attr.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_dev_attr.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_dev_attr.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_dev_attr.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_dev_attr.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_dev_attr.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_dev_attr.c"

$(IntermediateDirectory)/wps_wps_enrollee.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_enrollee.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_enrollee.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_enrollee.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_enrollee.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_enrollee.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_enrollee.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_enrollee.c"

$(IntermediateDirectory)/wps_wps_hostapd.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_hostapd.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_hostapd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_hostapd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_hostapd.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_hostapd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_hostapd.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_hostapd.c"

$(IntermediateDirectory)/wps_wps_main.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_main.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_main.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_main.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_main.c"

$(IntermediateDirectory)/wps_wps_registrar.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_registrar.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_registrar.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_registrar.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_registrar.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_registrar.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_registrar.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_registrar.c"

$(IntermediateDirectory)/wps_wps_sta.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_sta.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_sta.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_sta.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_sta.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_sta.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_sta.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_sta.c"

$(IntermediateDirectory)/wps_wps_validate.c$(ObjectSuffix): ../../net/atbm/net/wpa/wps/wps_validate.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/net/atbm/net/wpa/wps/wps_validate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wps_wps_validate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wps_wps_validate.c$(PreprocessSuffix): ../../net/atbm/net/wpa/wps/wps_validate.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wps_wps_validate.c$(PreprocessSuffix) "../../net/atbm/net/wpa/wps/wps_validate.c"

##
## Clean
##
clean:
	$(RM) -r Debug/


