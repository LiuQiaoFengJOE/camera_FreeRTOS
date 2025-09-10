##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-lwip
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AX3292-lwip.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../arch/AX3292/inc $(IncludeSwitch)../../inc $(IncludeSwitch)../../os/lwip/2.0.2/include $(IncludeSwitch)../../os/lwip/ports/AX3292 $(IncludeSwitch)../../board/AX3292/mvl8801 
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
CFLAGS   :=  -g -Os -Wall -mhard-div -ffunction-sections -fdata-sections $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.7.1/bin/or1k-elf-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/netif_ethernet.c$(ObjectSuffix) $(IntermediateDirectory)/netif_ethernetif.c$(ObjectSuffix) $(IntermediateDirectory)/netif_lowpan6.c$(ObjectSuffix) $(IntermediateDirectory)/netif_slipif.c$(ObjectSuffix) $(IntermediateDirectory)/core_def.c$(ObjectSuffix) $(IntermediateDirectory)/core_dns.c$(ObjectSuffix) $(IntermediateDirectory)/core_inet_chksum.c$(ObjectSuffix) $(IntermediateDirectory)/core_init.c$(ObjectSuffix) $(IntermediateDirectory)/core_ip.c$(ObjectSuffix) $(IntermediateDirectory)/core_mem.c$(ObjectSuffix) \
	$(IntermediateDirectory)/core_memp.c$(ObjectSuffix) $(IntermediateDirectory)/core_netif.c$(ObjectSuffix) $(IntermediateDirectory)/core_pbuf.c$(ObjectSuffix) $(IntermediateDirectory)/core_raw.c$(ObjectSuffix) $(IntermediateDirectory)/core_stats.c$(ObjectSuffix) $(IntermediateDirectory)/core_sys.c$(ObjectSuffix) $(IntermediateDirectory)/core_tcp.c$(ObjectSuffix) $(IntermediateDirectory)/core_tcp_in.c$(ObjectSuffix) $(IntermediateDirectory)/core_tcp_out.c$(ObjectSuffix) $(IntermediateDirectory)/core_timeouts.c$(ObjectSuffix) \
	$(IntermediateDirectory)/core_udp.c$(ObjectSuffix) $(IntermediateDirectory)/api_api_lib.c$(ObjectSuffix) $(IntermediateDirectory)/api_api_msg.c$(ObjectSuffix) $(IntermediateDirectory)/api_err.c$(ObjectSuffix) $(IntermediateDirectory)/api_netbuf.c$(ObjectSuffix) $(IntermediateDirectory)/api_netdb.c$(ObjectSuffix) $(IntermediateDirectory)/api_netifapi.c$(ObjectSuffix) $(IntermediateDirectory)/api_sockets.c$(ObjectSuffix) $(IntermediateDirectory)/api_tcpip.c$(ObjectSuffix) $(IntermediateDirectory)/arch_sys_arch.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ppp_auth.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_ccp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_chap-md5.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_chap-new.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_chap_ms.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_demand.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_eap.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_ecp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_eui64.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_fsm.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ppp_ipcp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_ipv6cp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_lcp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_magic.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_mppe.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_multilink.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_ppp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppapi.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppcrypt.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppoe.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ppp_pppol2tp.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_pppos.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_upap.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_utils.c$(ObjectSuffix) $(IntermediateDirectory)/ppp_vj.c$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/ipv4_autoip.c$(ObjectSuffix) $(IntermediateDirectory)/ipv4_dhcp.c$(ObjectSuffix) $(IntermediateDirectory)/ipv4_etharp.c$(ObjectSuffix) $(IntermediateDirectory)/ipv4_icmp.c$(ObjectSuffix) $(IntermediateDirectory)/ipv4_igmp.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ipv4_ip4.c$(ObjectSuffix) $(IntermediateDirectory)/ipv4_ip4_addr.c$(ObjectSuffix) $(IntermediateDirectory)/ipv4_ip4_frag.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_dhcp6.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ethip6.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_icmp6.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_inet6.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ip6.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ip6_addr.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_ip6_frag.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ipv6_mld6.c$(ObjectSuffix) $(IntermediateDirectory)/ipv6_nd6.c$(ObjectSuffix) $(IntermediateDirectory)/polarssl_arc4.c$(ObjectSuffix) $(IntermediateDirectory)/polarssl_des.c$(ObjectSuffix) $(IntermediateDirectory)/polarssl_md4.c$(ObjectSuffix) $(IntermediateDirectory)/polarssl_md5.c$(ObjectSuffix) $(IntermediateDirectory)/polarssl_sha1.c$(ObjectSuffix) 



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
	@echo rebuilt > "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug/AX3292-lwip"

./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/netif_ethernet.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ethernet.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ethernet.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/netif_ethernet.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/netif_ethernet.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ethernet.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/netif_ethernet.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ethernet.c"

$(IntermediateDirectory)/netif_ethernetif.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ethernetif.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ethernetif.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/netif_ethernetif.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/netif_ethernetif.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ethernetif.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/netif_ethernetif.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ethernetif.c"

$(IntermediateDirectory)/netif_lowpan6.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/lowpan6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/lowpan6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/netif_lowpan6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/netif_lowpan6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/lowpan6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/netif_lowpan6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/lowpan6.c"

$(IntermediateDirectory)/netif_slipif.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/slipif.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/slipif.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/netif_slipif.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/netif_slipif.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/slipif.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/netif_slipif.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/slipif.c"

$(IntermediateDirectory)/core_def.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/def.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/def.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_def.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_def.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/def.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_def.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/def.c"

$(IntermediateDirectory)/core_dns.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/dns.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/dns.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_dns.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_dns.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/dns.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_dns.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/dns.c"

$(IntermediateDirectory)/core_inet_chksum.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/inet_chksum.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/inet_chksum.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_inet_chksum.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_inet_chksum.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/inet_chksum.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_inet_chksum.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/inet_chksum.c"

$(IntermediateDirectory)/core_init.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/init.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/init.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_init.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_init.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/init.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_init.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/init.c"

$(IntermediateDirectory)/core_ip.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ip.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ip.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_ip.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_ip.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ip.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_ip.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ip.c"

$(IntermediateDirectory)/core_mem.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/mem.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/mem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_mem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_mem.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/mem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_mem.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/mem.c"

$(IntermediateDirectory)/core_memp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/memp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/memp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_memp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_memp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/memp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_memp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/memp.c"

$(IntermediateDirectory)/core_netif.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/netif.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/netif.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_netif.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_netif.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/netif.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_netif.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/netif.c"

$(IntermediateDirectory)/core_pbuf.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/pbuf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/pbuf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_pbuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_pbuf.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/pbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_pbuf.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/pbuf.c"

$(IntermediateDirectory)/core_raw.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/raw.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/raw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_raw.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_raw.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/raw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_raw.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/raw.c"

$(IntermediateDirectory)/core_stats.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/stats.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/stats.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_stats.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_stats.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/stats.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_stats.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/stats.c"

$(IntermediateDirectory)/core_sys.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/sys.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/sys.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_sys.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_sys.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/sys.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_sys.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/sys.c"

$(IntermediateDirectory)/core_tcp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/tcp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/tcp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_tcp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_tcp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/tcp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_tcp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/tcp.c"

$(IntermediateDirectory)/core_tcp_in.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/tcp_in.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/tcp_in.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_tcp_in.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_tcp_in.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/tcp_in.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_tcp_in.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/tcp_in.c"

$(IntermediateDirectory)/core_tcp_out.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/tcp_out.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/tcp_out.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_tcp_out.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_tcp_out.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/tcp_out.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_tcp_out.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/tcp_out.c"

$(IntermediateDirectory)/core_timeouts.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/timeouts.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/timeouts.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_timeouts.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_timeouts.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/timeouts.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_timeouts.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/timeouts.c"

$(IntermediateDirectory)/core_udp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/udp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/udp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_udp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_udp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/udp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_udp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/udp.c"

$(IntermediateDirectory)/api_api_lib.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/api_lib.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/api_lib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_api_lib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_api_lib.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/api_lib.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_api_lib.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/api_lib.c"

$(IntermediateDirectory)/api_api_msg.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/api_msg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/api_msg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_api_msg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_api_msg.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/api_msg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_api_msg.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/api_msg.c"

$(IntermediateDirectory)/api_err.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/err.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/err.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_err.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_err.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/err.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_err.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/err.c"

$(IntermediateDirectory)/api_netbuf.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/netbuf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/netbuf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_netbuf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_netbuf.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/netbuf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_netbuf.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/netbuf.c"

$(IntermediateDirectory)/api_netdb.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/netdb.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/netdb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_netdb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_netdb.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/netdb.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_netdb.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/netdb.c"

$(IntermediateDirectory)/api_netifapi.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/netifapi.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/netifapi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_netifapi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_netifapi.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/netifapi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_netifapi.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/netifapi.c"

$(IntermediateDirectory)/api_sockets.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/sockets.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/sockets.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_sockets.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_sockets.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/sockets.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_sockets.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/sockets.c"

$(IntermediateDirectory)/api_tcpip.c$(ObjectSuffix): ../../os/lwip/2.0.2/api/tcpip.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/api/tcpip.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/api_tcpip.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/api_tcpip.c$(PreprocessSuffix): ../../os/lwip/2.0.2/api/tcpip.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/api_tcpip.c$(PreprocessSuffix) "../../os/lwip/2.0.2/api/tcpip.c"

$(IntermediateDirectory)/arch_sys_arch.c$(ObjectSuffix): ../../os/lwip/ports/AX3292/arch/sys_arch.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/ports/AX3292/arch/sys_arch.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/arch_sys_arch.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/arch_sys_arch.c$(PreprocessSuffix): ../../os/lwip/ports/AX3292/arch/sys_arch.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/arch_sys_arch.c$(PreprocessSuffix) "../../os/lwip/ports/AX3292/arch/sys_arch.c"

$(IntermediateDirectory)/ppp_auth.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/auth.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/auth.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_auth.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_auth.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/auth.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_auth.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/auth.c"

$(IntermediateDirectory)/ppp_ccp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/ccp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/ccp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ccp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_ccp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/ccp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_ccp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/ccp.c"

$(IntermediateDirectory)/ppp_chap-md5.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/chap-md5.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/chap-md5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_chap-md5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_chap-md5.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/chap-md5.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_chap-md5.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/chap-md5.c"

$(IntermediateDirectory)/ppp_chap-new.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/chap-new.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/chap-new.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_chap-new.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_chap-new.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/chap-new.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_chap-new.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/chap-new.c"

$(IntermediateDirectory)/ppp_chap_ms.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/chap_ms.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/chap_ms.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_chap_ms.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_chap_ms.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/chap_ms.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_chap_ms.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/chap_ms.c"

$(IntermediateDirectory)/ppp_demand.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/demand.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/demand.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_demand.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_demand.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/demand.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_demand.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/demand.c"

$(IntermediateDirectory)/ppp_eap.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/eap.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/eap.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_eap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_eap.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/eap.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_eap.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/eap.c"

$(IntermediateDirectory)/ppp_ecp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/ecp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/ecp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ecp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_ecp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/ecp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_ecp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/ecp.c"

$(IntermediateDirectory)/ppp_eui64.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/eui64.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/eui64.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_eui64.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_eui64.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/eui64.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_eui64.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/eui64.c"

$(IntermediateDirectory)/ppp_fsm.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/fsm.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/fsm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_fsm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_fsm.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/fsm.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_fsm.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/fsm.c"

$(IntermediateDirectory)/ppp_ipcp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/ipcp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/ipcp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ipcp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_ipcp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/ipcp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_ipcp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/ipcp.c"

$(IntermediateDirectory)/ppp_ipv6cp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/ipv6cp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/ipv6cp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ipv6cp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_ipv6cp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/ipv6cp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_ipv6cp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/ipv6cp.c"

$(IntermediateDirectory)/ppp_lcp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/lcp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/lcp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_lcp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_lcp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/lcp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_lcp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/lcp.c"

$(IntermediateDirectory)/ppp_magic.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/magic.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/magic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_magic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_magic.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/magic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_magic.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/magic.c"

$(IntermediateDirectory)/ppp_mppe.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/mppe.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/mppe.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_mppe.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_mppe.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/mppe.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_mppe.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/mppe.c"

$(IntermediateDirectory)/ppp_multilink.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/multilink.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/multilink.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_multilink.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_multilink.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/multilink.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_multilink.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/multilink.c"

$(IntermediateDirectory)/ppp_ppp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/ppp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/ppp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_ppp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_ppp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/ppp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_ppp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/ppp.c"

$(IntermediateDirectory)/ppp_pppapi.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/pppapi.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/pppapi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppapi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_pppapi.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/pppapi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_pppapi.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/pppapi.c"

$(IntermediateDirectory)/ppp_pppcrypt.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/pppcrypt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/pppcrypt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppcrypt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_pppcrypt.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/pppcrypt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_pppcrypt.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/pppcrypt.c"

$(IntermediateDirectory)/ppp_pppoe.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/pppoe.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/pppoe.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppoe.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_pppoe.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/pppoe.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_pppoe.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/pppoe.c"

$(IntermediateDirectory)/ppp_pppol2tp.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/pppol2tp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/pppol2tp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppol2tp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_pppol2tp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/pppol2tp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_pppol2tp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/pppol2tp.c"

$(IntermediateDirectory)/ppp_pppos.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/pppos.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/pppos.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_pppos.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_pppos.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/pppos.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_pppos.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/pppos.c"

$(IntermediateDirectory)/ppp_upap.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/upap.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/upap.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_upap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_upap.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/upap.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_upap.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/upap.c"

$(IntermediateDirectory)/ppp_utils.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/utils.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/utils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_utils.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_utils.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/utils.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_utils.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/utils.c"

$(IntermediateDirectory)/ppp_vj.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/vj.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/vj.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ppp_vj.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ppp_vj.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/vj.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ppp_vj.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/vj.c"

$(IntermediateDirectory)/ipv4_autoip.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/autoip.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/autoip.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_autoip.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_autoip.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/autoip.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_autoip.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/autoip.c"

$(IntermediateDirectory)/ipv4_dhcp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/dhcp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/dhcp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_dhcp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_dhcp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/dhcp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_dhcp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/dhcp.c"

$(IntermediateDirectory)/ipv4_etharp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/etharp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/etharp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_etharp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_etharp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/etharp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_etharp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/etharp.c"

$(IntermediateDirectory)/ipv4_icmp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/icmp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/icmp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_icmp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_icmp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/icmp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_icmp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/icmp.c"

$(IntermediateDirectory)/ipv4_igmp.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/igmp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/igmp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_igmp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_igmp.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/igmp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_igmp.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/igmp.c"

$(IntermediateDirectory)/ipv4_ip4.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/ip4.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/ip4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_ip4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_ip4.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/ip4.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_ip4.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/ip4.c"

$(IntermediateDirectory)/ipv4_ip4_addr.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/ip4_addr.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/ip4_addr.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_ip4_addr.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_ip4_addr.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/ip4_addr.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_ip4_addr.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/ip4_addr.c"

$(IntermediateDirectory)/ipv4_ip4_frag.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv4/ip4_frag.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv4/ip4_frag.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv4_ip4_frag.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv4_ip4_frag.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv4/ip4_frag.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv4_ip4_frag.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv4/ip4_frag.c"

$(IntermediateDirectory)/ipv6_dhcp6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/dhcp6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/dhcp6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_dhcp6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_dhcp6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/dhcp6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_dhcp6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/dhcp6.c"

$(IntermediateDirectory)/ipv6_ethip6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/ethip6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/ethip6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ethip6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_ethip6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/ethip6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_ethip6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/ethip6.c"

$(IntermediateDirectory)/ipv6_icmp6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/icmp6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/icmp6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_icmp6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_icmp6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/icmp6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_icmp6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/icmp6.c"

$(IntermediateDirectory)/ipv6_inet6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/inet6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/inet6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_inet6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_inet6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/inet6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_inet6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/inet6.c"

$(IntermediateDirectory)/ipv6_ip6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/ip6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/ip6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ip6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_ip6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/ip6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_ip6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/ip6.c"

$(IntermediateDirectory)/ipv6_ip6_addr.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/ip6_addr.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/ip6_addr.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ip6_addr.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_ip6_addr.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/ip6_addr.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_ip6_addr.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/ip6_addr.c"

$(IntermediateDirectory)/ipv6_ip6_frag.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/ip6_frag.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/ip6_frag.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_ip6_frag.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_ip6_frag.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/ip6_frag.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_ip6_frag.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/ip6_frag.c"

$(IntermediateDirectory)/ipv6_mld6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/mld6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/mld6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_mld6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_mld6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/mld6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_mld6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/mld6.c"

$(IntermediateDirectory)/ipv6_nd6.c$(ObjectSuffix): ../../os/lwip/2.0.2/core/ipv6/nd6.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/core/ipv6/nd6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ipv6_nd6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ipv6_nd6.c$(PreprocessSuffix): ../../os/lwip/2.0.2/core/ipv6/nd6.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ipv6_nd6.c$(PreprocessSuffix) "../../os/lwip/2.0.2/core/ipv6/nd6.c"

$(IntermediateDirectory)/polarssl_arc4.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/arc4.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/polarssl/arc4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_arc4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/polarssl_arc4.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/arc4.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/polarssl_arc4.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/polarssl/arc4.c"

$(IntermediateDirectory)/polarssl_des.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/des.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/polarssl/des.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_des.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/polarssl_des.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/des.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/polarssl_des.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/polarssl/des.c"

$(IntermediateDirectory)/polarssl_md4.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/md4.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/polarssl/md4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_md4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/polarssl_md4.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/md4.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/polarssl_md4.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/polarssl/md4.c"

$(IntermediateDirectory)/polarssl_md5.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/md5.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/polarssl/md5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_md5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/polarssl_md5.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/md5.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/polarssl_md5.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/polarssl/md5.c"

$(IntermediateDirectory)/polarssl_sha1.c$(ObjectSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/sha1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/os/lwip/2.0.2/netif/ppp/polarssl/sha1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/polarssl_sha1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/polarssl_sha1.c$(PreprocessSuffix): ../../os/lwip/2.0.2/netif/ppp/polarssl/sha1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/polarssl_sha1.c$(PreprocessSuffix) "../../os/lwip/2.0.2/netif/ppp/polarssl/sha1.c"

##
## Clean
##
clean:
	$(RM) -r ./Debug/


