##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=lib_ax3268_atbm
ConfigurationName      :=Debug
WorkspacePath          := "D:\SVN\gt\appoplat"
ProjectPath            := "D:\SVN\gt\appoplat\drivers\thirddriver\wifi\atbm"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=hamin
Date                   :=10/04/2019
CodeLitePath           :="C:\Program Files\CodeLite"
LinkerName             :=or1k-elf-ld
SharedObjectLinkerName :=or1k-elf-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="lib_ax3268_atbm.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./ $(IncludeSwitch)../ $(IncludeSwitch)../../../ $(IncludeSwitch)../../../../ $(IncludeSwitch)../../../../os/linux $(IncludeSwitch)../../../../os/freeRTOS/api/inc $(IncludeSwitch)../../../../os/freeRTOS/kernel/inc $(IncludeSwitch)../../../../os/freeRTOS/ports/openrisc $(IncludeSwitch)../../../../app/inc $(IncludeSwitch)../../../../config $(IncludeSwitch)../../../../net/lwip/src/include/ipv4 $(IncludeSwitch)../../../../net/lwip/src/include $(IncludeSwitch)../../../../net/lwip/src/arch $(IncludeSwitch)../../../../net/lwip/src $(IncludeSwitch)../../../../net/udhcp $(IncludeSwitch)../../../../drivers/inc $(IncludeSwitch)../../../../drivers/uart $(IncludeSwitch)../../../../drivers/sdc $(IncludeSwitch)../../../../drivers $(IncludeSwitch)../../../../net/udhcp-0.9.8 $(IncludeSwitch)../../../../net/wpa $(IncludeSwitch)../../../../net/wireless $(IncludeSwitch)../../../../net/http $(IncludeSwitch)../../../../net/spook $(IncludeSwitch)../../../../net/eloop $(IncludeSwitch)../../../../net/lwip/src/include/lwip $(IncludeSwitch)../../../../drivers/sd/sdcard $(IncludeSwitch)../../../../drivers/sd/sdio $(IncludeSwitch)../../../../drivers/appodriver/sdio $(IncludeSwitch)../../../../inc $(IncludeSwitch)../../../../inc/hld/sdio $(IncludeSwitch)../../../../inc/board/AX3268_atbm $(IncludeSwitch)../../../../inc/system/AX3268 $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/include $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/hal/include $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/hal/sdio $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/net/include $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/net/include/proto $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/os/ankai_os/include $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/os/include $(IncludeSwitch)../../../../drivers/thirddriver/wifi/atbm/api 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)C:/MinGW-4.7.1/lib/gcc/or1k-elf/4.9.1 $(LibraryPathSwitch)C:/MinGW-4.7.1/or1k-elf/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := or1k-elf-ar rcu
CXX      := or1k-elf-g++
CC       := or1k-elf-gcc
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=  -g -O3 -Wall -fshort-wchar -Wno-pointer-sign -Wno-format -Wno-array-bounds -Wno-strict-aliasing -ffunction-sections -fdata-sections $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\MinGW-4.7.1\bin;$(PATH)
Objects0=$(IntermediateDirectory)/net_app_wifi_cmd.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix) $(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix) \
	$(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_smartconfig.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix) $(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix) \
	$(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix) $(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix) $(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_api.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(ObjectSuffix) $(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix) $(IntermediateDirectory)/wpa_wpa_timer.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix) $(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix) $(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(ObjectSuffix) $(IntermediateDirectory)/ak_os_api_wifi_netif.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "D:\SVN\gt\appoplat/.build-debug"
	@echo rebuilt > "D:\SVN\gt\appoplat/.build-debug/lib_ax3268_atbm"

PostBuild:
	@echo Executing Post Build commands ...
	copy /Y .\Debug\lib$(ProjectName).a ..\..\..\..\lib\lib_atbm.a
	@echo Done

./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/net_app_wifi_cmd.c$(ObjectSuffix): net/app_wifi_cmd.c $(IntermediateDirectory)/net_app_wifi_cmd.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_app_wifi_cmd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_app_wifi_cmd.c$(DependSuffix): net/app_wifi_cmd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_app_wifi_cmd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_app_wifi_cmd.c$(DependSuffix) -MM "net/app_wifi_cmd.c"

$(IntermediateDirectory)/net_app_wifi_cmd.c$(PreprocessSuffix): net/app_wifi_cmd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_app_wifi_cmd.c$(PreprocessSuffix) "net/app_wifi_cmd.c"

$(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix): net/atbm_key.c $(IntermediateDirectory)/net_atbm_key.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_key.c$(DependSuffix): net/atbm_key.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_key.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_key.c$(DependSuffix) -MM "net/atbm_key.c"

$(IntermediateDirectory)/net_atbm_key.c$(PreprocessSuffix): net/atbm_key.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_key.c$(PreprocessSuffix) "net/atbm_key.c"

$(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix): net/atbm_mgmt.c $(IntermediateDirectory)/net_atbm_mgmt.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_mgmt.c$(DependSuffix): net/atbm_mgmt.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_mgmt.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_mgmt.c$(DependSuffix) -MM "net/atbm_mgmt.c"

$(IntermediateDirectory)/net_atbm_mgmt.c$(PreprocessSuffix): net/atbm_mgmt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_mgmt.c$(PreprocessSuffix) "net/atbm_mgmt.c"

$(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix): net/atbm_ratectrl.c $(IntermediateDirectory)/net_atbm_ratectrl.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_ratectrl.c$(DependSuffix): net/atbm_ratectrl.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_ratectrl.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_ratectrl.c$(DependSuffix) -MM "net/atbm_ratectrl.c"

$(IntermediateDirectory)/net_atbm_ratectrl.c$(PreprocessSuffix): net/atbm_ratectrl.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_ratectrl.c$(PreprocessSuffix) "net/atbm_ratectrl.c"

$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix): net/atbm_rc80211_minstrel.c $(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(DependSuffix): net/atbm_rc80211_minstrel.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(DependSuffix) -MM "net/atbm_rc80211_minstrel.c"

$(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(PreprocessSuffix): net/atbm_rc80211_minstrel.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_rc80211_minstrel.c$(PreprocessSuffix) "net/atbm_rc80211_minstrel.c"

$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix): net/atbm_rc80211_minstrel_ht.c $(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(DependSuffix): net/atbm_rc80211_minstrel_ht.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(DependSuffix) -MM "net/atbm_rc80211_minstrel_ht.c"

$(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(PreprocessSuffix): net/atbm_rc80211_minstrel_ht.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_rc80211_minstrel_ht.c$(PreprocessSuffix) "net/atbm_rc80211_minstrel_ht.c"

$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix): net/atbm_rc80211_pid_algo.c $(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(DependSuffix): net/atbm_rc80211_pid_algo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(DependSuffix) -MM "net/atbm_rc80211_pid_algo.c"

$(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(PreprocessSuffix): net/atbm_rc80211_pid_algo.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_rc80211_pid_algo.c$(PreprocessSuffix) "net/atbm_rc80211_pid_algo.c"

$(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix): net/atbm_util.c $(IntermediateDirectory)/net_atbm_util.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/net_atbm_util.c$(DependSuffix): net/atbm_util.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/net_atbm_util.c$(ObjectSuffix) -MF$(IntermediateDirectory)/net_atbm_util.c$(DependSuffix) -MM "net/atbm_util.c"

$(IntermediateDirectory)/net_atbm_util.c$(PreprocessSuffix): net/atbm_util.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/net_atbm_util.c$(PreprocessSuffix) "net/atbm_util.c"

$(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix): hal/atbm_ap.c $(IntermediateDirectory)/hal_atbm_ap.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_ap.c$(DependSuffix): hal/atbm_ap.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_ap.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_ap.c$(DependSuffix) -MM "hal/atbm_ap.c"

$(IntermediateDirectory)/hal_atbm_ap.c$(PreprocessSuffix): hal/atbm_ap.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_ap.c$(PreprocessSuffix) "hal/atbm_ap.c"

$(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix): hal/atbm_config.c $(IntermediateDirectory)/hal_atbm_config.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_config.c$(DependSuffix): hal/atbm_config.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_config.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_config.c$(DependSuffix) -MM "hal/atbm_config.c"

$(IntermediateDirectory)/hal_atbm_config.c$(PreprocessSuffix): hal/atbm_config.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_config.c$(PreprocessSuffix) "hal/atbm_config.c"

$(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix): hal/atbm_etf.c $(IntermediateDirectory)/hal_atbm_etf.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_etf.c$(DependSuffix): hal/atbm_etf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_etf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_etf.c$(DependSuffix) -MM "hal/atbm_etf.c"

$(IntermediateDirectory)/hal_atbm_etf.c$(PreprocessSuffix): hal/atbm_etf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_etf.c$(PreprocessSuffix) "hal/atbm_etf.c"

$(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix): hal/atbm_init.c $(IntermediateDirectory)/hal_atbm_init.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_init.c$(DependSuffix): hal/atbm_init.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_init.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_init.c$(DependSuffix) -MM "hal/atbm_init.c"

$(IntermediateDirectory)/hal_atbm_init.c$(PreprocessSuffix): hal/atbm_init.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_init.c$(PreprocessSuffix) "hal/atbm_init.c"

$(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix): hal/atbm_main.c $(IntermediateDirectory)/hal_atbm_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_main.c$(DependSuffix): hal/atbm_main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_main.c$(DependSuffix) -MM "hal/atbm_main.c"

$(IntermediateDirectory)/hal_atbm_main.c$(PreprocessSuffix): hal/atbm_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_main.c$(PreprocessSuffix) "hal/atbm_main.c"

$(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix): hal/atbm_queue.c $(IntermediateDirectory)/hal_atbm_queue.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_queue.c$(DependSuffix): hal/atbm_queue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_queue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_queue.c$(DependSuffix) -MM "hal/atbm_queue.c"

$(IntermediateDirectory)/hal_atbm_queue.c$(PreprocessSuffix): hal/atbm_queue.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_queue.c$(PreprocessSuffix) "hal/atbm_queue.c"

$(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix): hal/atbm_skbuf.c $(IntermediateDirectory)/hal_atbm_skbuf.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_skbuf.c$(DependSuffix): hal/atbm_skbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_skbuf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_skbuf.c$(DependSuffix) -MM "hal/atbm_skbuf.c"

$(IntermediateDirectory)/hal_atbm_skbuf.c$(PreprocessSuffix): hal/atbm_skbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_skbuf.c$(PreprocessSuffix) "hal/atbm_skbuf.c"

$(IntermediateDirectory)/hal_atbm_smartconfig.c$(ObjectSuffix): hal/atbm_smartconfig.c $(IntermediateDirectory)/hal_atbm_smartconfig.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_smartconfig.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_smartconfig.c$(DependSuffix): hal/atbm_smartconfig.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_smartconfig.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_smartconfig.c$(DependSuffix) -MM "hal/atbm_smartconfig.c"

$(IntermediateDirectory)/hal_atbm_smartconfig.c$(PreprocessSuffix): hal/atbm_smartconfig.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_smartconfig.c$(PreprocessSuffix) "hal/atbm_smartconfig.c"

$(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix): hal/atbm_sta.c $(IntermediateDirectory)/hal_atbm_sta.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_sta.c$(DependSuffix): hal/atbm_sta.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_sta.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_sta.c$(DependSuffix) -MM "hal/atbm_sta.c"

$(IntermediateDirectory)/hal_atbm_sta.c$(PreprocessSuffix): hal/atbm_sta.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_sta.c$(PreprocessSuffix) "hal/atbm_sta.c"

$(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix): hal/atbm_task.c $(IntermediateDirectory)/hal_atbm_task.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_task.c$(DependSuffix): hal/atbm_task.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_task.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_task.c$(DependSuffix) -MM "hal/atbm_task.c"

$(IntermediateDirectory)/hal_atbm_task.c$(PreprocessSuffix): hal/atbm_task.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_task.c$(PreprocessSuffix) "hal/atbm_task.c"

$(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix): hal/atbm_txrx.c $(IntermediateDirectory)/hal_atbm_txrx.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_txrx.c$(DependSuffix): hal/atbm_txrx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_txrx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_txrx.c$(DependSuffix) -MM "hal/atbm_txrx.c"

$(IntermediateDirectory)/hal_atbm_txrx.c$(PreprocessSuffix): hal/atbm_txrx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_txrx.c$(PreprocessSuffix) "hal/atbm_txrx.c"

$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix): hal/atbm_wifi_driver_api.c $(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(DependSuffix): hal/atbm_wifi_driver_api.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(DependSuffix) -MM "hal/atbm_wifi_driver_api.c"

$(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(PreprocessSuffix): hal/atbm_wifi_driver_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_wifi_driver_api.c$(PreprocessSuffix) "hal/atbm_wifi_driver_api.c"

$(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix): hal/atbm_wsm.c $(IntermediateDirectory)/hal_atbm_wsm.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_atbm_wsm.c$(DependSuffix): hal/atbm_wsm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_atbm_wsm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_atbm_wsm.c$(DependSuffix) -MM "hal/atbm_wsm.c"

$(IntermediateDirectory)/hal_atbm_wsm.c$(PreprocessSuffix): hal/atbm_wsm.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_atbm_wsm.c$(PreprocessSuffix) "hal/atbm_wsm.c"

$(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix): hal/smartconfig.c $(IntermediateDirectory)/hal_smartconfig.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hal_smartconfig.c$(DependSuffix): hal/smartconfig.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hal_smartconfig.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hal_smartconfig.c$(DependSuffix) -MM "hal/smartconfig.c"

$(IntermediateDirectory)/hal_smartconfig.c$(PreprocessSuffix): hal/smartconfig.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hal_smartconfig.c$(PreprocessSuffix) "hal/smartconfig.c"

$(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix): api/atbm_api.c $(IntermediateDirectory)/api_atbm_api.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_atbm_api.c$(DependSuffix): api/atbm_api.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/api_atbm_api.c$(ObjectSuffix) -MF$(IntermediateDirectory)/api_atbm_api.c$(DependSuffix) -MM "api/atbm_api.c"

$(IntermediateDirectory)/api_atbm_api.c$(PreprocessSuffix): api/atbm_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_atbm_api.c$(PreprocessSuffix) "api/atbm_api.c"

$(IntermediateDirectory)/ankai_os_atbm_os_api.c$(ObjectSuffix): os/ankai_os/atbm_os_api.c $(IntermediateDirectory)/ankai_os_atbm_os_api.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_api.c$(DependSuffix): os/ankai_os/atbm_os_api.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_api.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_api.c$(DependSuffix) -MM "os/ankai_os/atbm_os_api.c"

$(IntermediateDirectory)/ankai_os_atbm_os_api.c$(PreprocessSuffix): os/ankai_os/atbm_os_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_api.c$(PreprocessSuffix) "os/ankai_os/atbm_os_api.c"

$(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(ObjectSuffix): os/ankai_os/atbm_os_atomic.c $(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(DependSuffix): os/ankai_os/atbm_os_atomic.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(DependSuffix) -MM "os/ankai_os/atbm_os_atomic.c"

$(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(PreprocessSuffix): os/ankai_os/atbm_os_atomic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_atomic.c$(PreprocessSuffix) "os/ankai_os/atbm_os_atomic.c"

$(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(ObjectSuffix): os/ankai_os/atbm_os_mem.c $(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(DependSuffix): os/ankai_os/atbm_os_mem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(DependSuffix) -MM "os/ankai_os/atbm_os_mem.c"

$(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(PreprocessSuffix): os/ankai_os/atbm_os_mem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_mem.c$(PreprocessSuffix) "os/ankai_os/atbm_os_mem.c"

$(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(ObjectSuffix): os/ankai_os/atbm_os_mutex.c $(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(DependSuffix): os/ankai_os/atbm_os_mutex.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(DependSuffix) -MM "os/ankai_os/atbm_os_mutex.c"

$(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(PreprocessSuffix): os/ankai_os/atbm_os_mutex.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_mutex.c$(PreprocessSuffix) "os/ankai_os/atbm_os_mutex.c"

$(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(ObjectSuffix): os/ankai_os/atbm_os_sdio.c $(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(DependSuffix): os/ankai_os/atbm_os_sdio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(DependSuffix) -MM "os/ankai_os/atbm_os_sdio.c"

$(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(PreprocessSuffix): os/ankai_os/atbm_os_sdio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_sdio.c$(PreprocessSuffix) "os/ankai_os/atbm_os_sdio.c"

$(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(ObjectSuffix): os/ankai_os/atbm_os_skbuf.c $(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(DependSuffix): os/ankai_os/atbm_os_skbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(DependSuffix) -MM "os/ankai_os/atbm_os_skbuf.c"

$(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(PreprocessSuffix): os/ankai_os/atbm_os_skbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_skbuf.c$(PreprocessSuffix) "os/ankai_os/atbm_os_skbuf.c"

$(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(ObjectSuffix): os/ankai_os/atbm_os_spinlock.c $(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(DependSuffix): os/ankai_os/atbm_os_spinlock.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(DependSuffix) -MM "os/ankai_os/atbm_os_spinlock.c"

$(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(PreprocessSuffix): os/ankai_os/atbm_os_spinlock.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_spinlock.c$(PreprocessSuffix) "os/ankai_os/atbm_os_spinlock.c"

$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(ObjectSuffix): os/ankai_os/atbm_os_thread.c $(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(DependSuffix): os/ankai_os/atbm_os_thread.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(DependSuffix) -MM "os/ankai_os/atbm_os_thread.c"

$(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(PreprocessSuffix): os/ankai_os/atbm_os_thread.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_thread.c$(PreprocessSuffix) "os/ankai_os/atbm_os_thread.c"

$(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(ObjectSuffix): os/ankai_os/atbm_os_timer.c $(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(DependSuffix): os/ankai_os/atbm_os_timer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(DependSuffix) -MM "os/ankai_os/atbm_os_timer.c"

$(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(PreprocessSuffix): os/ankai_os/atbm_os_timer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_timer.c$(PreprocessSuffix) "os/ankai_os/atbm_os_timer.c"

$(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(ObjectSuffix): os/ankai_os/atbm_os_usb.c $(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(DependSuffix): os/ankai_os/atbm_os_usb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(DependSuffix) -MM "os/ankai_os/atbm_os_usb.c"

$(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(PreprocessSuffix): os/ankai_os/atbm_os_usb.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_usb.c$(PreprocessSuffix) "os/ankai_os/atbm_os_usb.c"

$(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(ObjectSuffix): os/ankai_os/atbm_os_workqueue.c $(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(DependSuffix): os/ankai_os/atbm_os_workqueue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(DependSuffix) -MM "os/ankai_os/atbm_os_workqueue.c"

$(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(PreprocessSuffix): os/ankai_os/atbm_os_workqueue.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ankai_os_atbm_os_workqueue.c$(PreprocessSuffix) "os/ankai_os/atbm_os_workqueue.c"

$(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix): net/wpa/aes_core.c $(IntermediateDirectory)/wpa_aes_core.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_aes_core.c$(DependSuffix): net/wpa/aes_core.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wpa_aes_core.c$(ObjectSuffix) -MF$(IntermediateDirectory)/wpa_aes_core.c$(DependSuffix) -MM "net/wpa/aes_core.c"

$(IntermediateDirectory)/wpa_aes_core.c$(PreprocessSuffix): net/wpa/aes_core.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_aes_core.c$(PreprocessSuffix) "net/wpa/aes_core.c"

$(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix): net/wpa/hostapd_main.c $(IntermediateDirectory)/wpa_hostapd_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_hostapd_main.c$(DependSuffix): net/wpa/hostapd_main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wpa_hostapd_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/wpa_hostapd_main.c$(DependSuffix) -MM "net/wpa/hostapd_main.c"

$(IntermediateDirectory)/wpa_hostapd_main.c$(PreprocessSuffix): net/wpa/hostapd_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_hostapd_main.c$(PreprocessSuffix) "net/wpa/hostapd_main.c"

$(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix): net/wpa/sha1.c $(IntermediateDirectory)/wpa_sha1.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_sha1.c$(DependSuffix): net/wpa/sha1.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wpa_sha1.c$(ObjectSuffix) -MF$(IntermediateDirectory)/wpa_sha1.c$(DependSuffix) -MM "net/wpa/sha1.c"

$(IntermediateDirectory)/wpa_sha1.c$(PreprocessSuffix): net/wpa/sha1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_sha1.c$(PreprocessSuffix) "net/wpa/sha1.c"

$(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix): net/wpa/wpa_common.c $(IntermediateDirectory)/wpa_wpa_common.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_common.c$(DependSuffix): net/wpa/wpa_common.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wpa_wpa_common.c$(ObjectSuffix) -MF$(IntermediateDirectory)/wpa_wpa_common.c$(DependSuffix) -MM "net/wpa/wpa_common.c"

$(IntermediateDirectory)/wpa_wpa_common.c$(PreprocessSuffix): net/wpa/wpa_common.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_common.c$(PreprocessSuffix) "net/wpa/wpa_common.c"

$(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix): net/wpa/wpa_main.c $(IntermediateDirectory)/wpa_wpa_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_main.c$(DependSuffix): net/wpa/wpa_main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wpa_wpa_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/wpa_wpa_main.c$(DependSuffix) -MM "net/wpa/wpa_main.c"

$(IntermediateDirectory)/wpa_wpa_main.c$(PreprocessSuffix): net/wpa/wpa_main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_main.c$(PreprocessSuffix) "net/wpa/wpa_main.c"

$(IntermediateDirectory)/wpa_wpa_timer.c$(ObjectSuffix): net/wpa/wpa_timer.c $(IntermediateDirectory)/wpa_wpa_timer.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wpa_wpa_timer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wpa_wpa_timer.c$(DependSuffix): net/wpa/wpa_timer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wpa_wpa_timer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/wpa_wpa_timer.c$(DependSuffix) -MM "net/wpa/wpa_timer.c"

$(IntermediateDirectory)/wpa_wpa_timer.c$(PreprocessSuffix): net/wpa/wpa_timer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wpa_wpa_timer.c$(PreprocessSuffix) "net/wpa/wpa_timer.c"

$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix): hal/sdio/atbm_dcxo_dpll.c $(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(DependSuffix): hal/sdio/atbm_dcxo_dpll.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(DependSuffix) -MM "hal/sdio/atbm_dcxo_dpll.c"

$(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(PreprocessSuffix): hal/sdio/atbm_dcxo_dpll.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_dcxo_dpll.c$(PreprocessSuffix) "hal/sdio/atbm_dcxo_dpll.c"

$(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix): hal/sdio/atbm_sdio.c $(IntermediateDirectory)/sdio_atbm_sdio.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio.c$(DependSuffix): hal/sdio/atbm_sdio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdio_atbm_sdio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sdio_atbm_sdio.c$(DependSuffix) -MM "hal/sdio/atbm_sdio.c"

$(IntermediateDirectory)/sdio_atbm_sdio.c$(PreprocessSuffix): hal/sdio/atbm_sdio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio.c$(PreprocessSuffix) "hal/sdio/atbm_sdio.c"

$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix): hal/sdio/atbm_sdio_bh.c $(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(DependSuffix): hal/sdio/atbm_sdio_bh.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(DependSuffix) -MM "hal/sdio/atbm_sdio_bh.c"

$(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(PreprocessSuffix): hal/sdio/atbm_sdio_bh.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio_bh.c$(PreprocessSuffix) "hal/sdio/atbm_sdio_bh.c"

$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix): hal/sdio/atbm_sdio_fwio.c $(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(DependSuffix): hal/sdio/atbm_sdio_fwio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(DependSuffix) -MM "hal/sdio/atbm_sdio_fwio.c"

$(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(PreprocessSuffix): hal/sdio/atbm_sdio_fwio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio_fwio.c$(PreprocessSuffix) "hal/sdio/atbm_sdio_fwio.c"

$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix): hal/sdio/atbm_sdio_hwio.c $(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(DependSuffix): hal/sdio/atbm_sdio_hwio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(DependSuffix) -MM "hal/sdio/atbm_sdio_hwio.c"

$(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(PreprocessSuffix): hal/sdio/atbm_sdio_hwio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdio_atbm_sdio_hwio.c$(PreprocessSuffix) "hal/sdio/atbm_sdio_hwio.c"

$(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(ObjectSuffix): os/ankai_os/ak_os_api/wifi_atbm_api.c $(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(DependSuffix): os/ankai_os/ak_os_api/wifi_atbm_api.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(DependSuffix) -MM "os/ankai_os/ak_os_api/wifi_atbm_api.c"

$(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(PreprocessSuffix): os/ankai_os/ak_os_api/wifi_atbm_api.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ak_os_api_wifi_atbm_api.c$(PreprocessSuffix) "os/ankai_os/ak_os_api/wifi_atbm_api.c"

$(IntermediateDirectory)/ak_os_api_wifi_netif.c$(ObjectSuffix): os/ankai_os/ak_os_api/wifi_netif.c $(IntermediateDirectory)/ak_os_api_wifi_netif.c$(DependSuffix)
	$(CC) $(SourceSwitch) $< $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ak_os_api_wifi_netif.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ak_os_api_wifi_netif.c$(DependSuffix): os/ankai_os/ak_os_api/wifi_netif.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ak_os_api_wifi_netif.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ak_os_api_wifi_netif.c$(DependSuffix) -MM "os/ankai_os/ak_os_api/wifi_netif.c"

$(IntermediateDirectory)/ak_os_api_wifi_netif.c$(PreprocessSuffix): os/ankai_os/ak_os_api/wifi_netif.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ak_os_api_wifi_netif.c$(PreprocessSuffix) "os/ankai_os/ak_os_api/wifi_netif.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


