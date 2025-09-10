##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-mcu
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
ObjectsFileList        :="AX3292-mcu.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../inc $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../arch/AX3292/inc $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../sys/drv $(IncludeSwitch)../../sys $(IncludeSwitch)../../lib $(IncludeSwitch)../../lib/drv $(IncludeSwitch)../../os/freeRTOS/inc $(IncludeSwitch)../../os/freeRTOS/osal $(IncludeSwitch)../../os/freeRTOS/ports/AX3292 $(IncludeSwitch)../../arch/AX3292/hal/inc/ $(IncludeSwitch)../../arch/AX3292/ax32xx/inc $(IncludeSwitch)../../board/AX3292/mvl8801 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)C:/MinGW-4.7.1/lib/gcc/or1k-elf/4.9.1/no-delay $(LibraryPathSwitch)C:/MinGW-4.7.1/or1k-elf/lib/no-delay  $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

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
Objects0=$(IntermediateDirectory)/adc_adc.c$(ObjectSuffix) $(IntermediateDirectory)/adc_hal_adc.c$(ObjectSuffix) $(IntermediateDirectory)/cache_cache.c$(ObjectSuffix) $(IntermediateDirectory)/csi_ax32xx_csi.c$(ObjectSuffix) $(IntermediateDirectory)/gpio_ops_gpio.c$(ObjectSuffix) $(IntermediateDirectory)/i2c_iic.c$(ObjectSuffix) $(IntermediateDirectory)/irq_interrupt.c$(ObjectSuffix) $(IntermediateDirectory)/irq_irq_s.S$(ObjectSuffix) $(IntermediateDirectory)/isp_ax32xx_isp.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_lcdc.c$(ObjectSuffix) \
	$(IntermediateDirectory)/lcd_osd.c$(ObjectSuffix) $(IntermediateDirectory)/lcd_rotate.c$(ObjectSuffix) $(IntermediateDirectory)/mem_mem.c$(ObjectSuffix) $(IntermediateDirectory)/mem_mem_s.S$(ObjectSuffix) $(IntermediateDirectory)/mjpeg_jpeg.c$(ObjectSuffix) $(IntermediateDirectory)/mjpeg_jpeg_table.c$(ObjectSuffix) $(IntermediateDirectory)/mjpeg_mjpeg.c$(ObjectSuffix) $(IntermediateDirectory)/sdc_sdc.c$(ObjectSuffix) $(IntermediateDirectory)/spi_spi0.c$(ObjectSuffix) $(IntermediateDirectory)/startup_BLDRX32.S$(ObjectSuffix) \
	$(IntermediateDirectory)/startup_boost.S$(ObjectSuffix) $(IntermediateDirectory)/startup_boot.S$(ObjectSuffix) $(IntermediateDirectory)/startup_bootsecs.c$(ObjectSuffix) $(IntermediateDirectory)/startup_DBR.S$(ObjectSuffix) $(IntermediateDirectory)/startup_MBR.S$(ObjectSuffix) $(IntermediateDirectory)/startup_vector.S$(ObjectSuffix) $(IntermediateDirectory)/sys_sys.c$(ObjectSuffix) $(IntermediateDirectory)/timer_timer0.c$(ObjectSuffix) $(IntermediateDirectory)/uart_uart0.c$(ObjectSuffix) $(IntermediateDirectory)/uart_btuart.c$(ObjectSuffix) \
	$(IntermediateDirectory)/usb_usb.c$(ObjectSuffix) $(IntermediateDirectory)/usb_237a_1.c$(ObjectSuffix) $(IntermediateDirectory)/usb_237a_2.c$(ObjectSuffix) $(IntermediateDirectory)/usb_usb11.c$(ObjectSuffix) $(IntermediateDirectory)/usb_jt525.c$(ObjectSuffix) $(IntermediateDirectory)/watchdog_watchdog.c$(ObjectSuffix) $(IntermediateDirectory)/ax32xx_rtc_ax32xx_rtc.c$(ObjectSuffix) $(IntermediateDirectory)/dac_ax32xx_dac.c$(ObjectSuffix) $(IntermediateDirectory)/pwm_pwm.c$(ObjectSuffix) $(IntermediateDirectory)/tminf_ax32xx_tminf.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_ax32xx_auadc.c$(ObjectSuffix) $(IntermediateDirectory)/src_ax32xx_dma.c$(ObjectSuffix) $(IntermediateDirectory)/src_ax32xx_wdt.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug"
	@echo rebuilt > "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build/.build-debug/AX3292-mcu"

./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/adc_adc.c$(ObjectSuffix): ../../arch/AX3292/adc/adc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/adc/adc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/adc_adc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/adc_adc.c$(PreprocessSuffix): ../../arch/AX3292/adc/adc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/adc_adc.c$(PreprocessSuffix) "../../arch/AX3292/adc/adc.c"

$(IntermediateDirectory)/adc_hal_adc.c$(ObjectSuffix): ../../arch/AX3292/adc/hal_adc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/adc/hal_adc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/adc_hal_adc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/adc_hal_adc.c$(PreprocessSuffix): ../../arch/AX3292/adc/hal_adc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/adc_hal_adc.c$(PreprocessSuffix) "../../arch/AX3292/adc/hal_adc.c"

$(IntermediateDirectory)/cache_cache.c$(ObjectSuffix): ../../arch/AX3292/cache/cache.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/cache/cache.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_cache.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_cache.c$(PreprocessSuffix): ../../arch/AX3292/cache/cache.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_cache.c$(PreprocessSuffix) "../../arch/AX3292/cache/cache.c"

$(IntermediateDirectory)/csi_ax32xx_csi.c$(ObjectSuffix): ../../arch/AX3292/csi/ax32xx_csi.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/csi/ax32xx_csi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csi_ax32xx_csi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csi_ax32xx_csi.c$(PreprocessSuffix): ../../arch/AX3292/csi/ax32xx_csi.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csi_ax32xx_csi.c$(PreprocessSuffix) "../../arch/AX3292/csi/ax32xx_csi.c"

$(IntermediateDirectory)/gpio_ops_gpio.c$(ObjectSuffix): ../../arch/AX3292/gpio_ops/gpio.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/gpio_ops/gpio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gpio_ops_gpio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/gpio_ops_gpio.c$(PreprocessSuffix): ../../arch/AX3292/gpio_ops/gpio.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/gpio_ops_gpio.c$(PreprocessSuffix) "../../arch/AX3292/gpio_ops/gpio.c"

$(IntermediateDirectory)/i2c_iic.c$(ObjectSuffix): ../../arch/AX3292/i2c/iic.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/i2c/iic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/i2c_iic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/i2c_iic.c$(PreprocessSuffix): ../../arch/AX3292/i2c/iic.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/i2c_iic.c$(PreprocessSuffix) "../../arch/AX3292/i2c/iic.c"

$(IntermediateDirectory)/irq_interrupt.c$(ObjectSuffix): ../../arch/AX3292/irq/interrupt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/irq/interrupt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/irq_interrupt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/irq_interrupt.c$(PreprocessSuffix): ../../arch/AX3292/irq/interrupt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/irq_interrupt.c$(PreprocessSuffix) "../../arch/AX3292/irq/interrupt.c"

$(IntermediateDirectory)/irq_irq_s.S$(ObjectSuffix): ../../arch/AX3292/irq/irq_s.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/irq/irq_s.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/irq_irq_s.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/irq_irq_s.S$(PreprocessSuffix): ../../arch/AX3292/irq/irq_s.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/irq_irq_s.S$(PreprocessSuffix) "../../arch/AX3292/irq/irq_s.S"

$(IntermediateDirectory)/isp_ax32xx_isp.c$(ObjectSuffix): ../../arch/AX3292/isp/ax32xx_isp.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/isp/ax32xx_isp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/isp_ax32xx_isp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/isp_ax32xx_isp.c$(PreprocessSuffix): ../../arch/AX3292/isp/ax32xx_isp.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/isp_ax32xx_isp.c$(PreprocessSuffix) "../../arch/AX3292/isp/ax32xx_isp.c"

$(IntermediateDirectory)/lcd_lcdc.c$(ObjectSuffix): ../../arch/AX3292/lcd/lcdc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/lcd/lcdc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_lcdc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_lcdc.c$(PreprocessSuffix): ../../arch/AX3292/lcd/lcdc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_lcdc.c$(PreprocessSuffix) "../../arch/AX3292/lcd/lcdc.c"

$(IntermediateDirectory)/lcd_osd.c$(ObjectSuffix): ../../arch/AX3292/lcd/osd.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/lcd/osd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_osd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_osd.c$(PreprocessSuffix): ../../arch/AX3292/lcd/osd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_osd.c$(PreprocessSuffix) "../../arch/AX3292/lcd/osd.c"

$(IntermediateDirectory)/lcd_rotate.c$(ObjectSuffix): ../../arch/AX3292/lcd/rotate.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/lcd/rotate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lcd_rotate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lcd_rotate.c$(PreprocessSuffix): ../../arch/AX3292/lcd/rotate.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lcd_rotate.c$(PreprocessSuffix) "../../arch/AX3292/lcd/rotate.c"

$(IntermediateDirectory)/mem_mem.c$(ObjectSuffix): ../../arch/AX3292/mem/mem.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/mem/mem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mem_mem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mem_mem.c$(PreprocessSuffix): ../../arch/AX3292/mem/mem.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mem_mem.c$(PreprocessSuffix) "../../arch/AX3292/mem/mem.c"

$(IntermediateDirectory)/mem_mem_s.S$(ObjectSuffix): ../../arch/AX3292/mem/mem_s.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/mem/mem_s.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mem_mem_s.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mem_mem_s.S$(PreprocessSuffix): ../../arch/AX3292/mem/mem_s.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mem_mem_s.S$(PreprocessSuffix) "../../arch/AX3292/mem/mem_s.S"

$(IntermediateDirectory)/mjpeg_jpeg.c$(ObjectSuffix): ../../arch/AX3292/mjpeg/jpeg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/mjpeg/jpeg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mjpeg_jpeg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mjpeg_jpeg.c$(PreprocessSuffix): ../../arch/AX3292/mjpeg/jpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mjpeg_jpeg.c$(PreprocessSuffix) "../../arch/AX3292/mjpeg/jpeg.c"

$(IntermediateDirectory)/mjpeg_jpeg_table.c$(ObjectSuffix): ../../arch/AX3292/mjpeg/jpeg_table.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/mjpeg/jpeg_table.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mjpeg_jpeg_table.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mjpeg_jpeg_table.c$(PreprocessSuffix): ../../arch/AX3292/mjpeg/jpeg_table.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mjpeg_jpeg_table.c$(PreprocessSuffix) "../../arch/AX3292/mjpeg/jpeg_table.c"

$(IntermediateDirectory)/mjpeg_mjpeg.c$(ObjectSuffix): ../../arch/AX3292/mjpeg/mjpeg.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/mjpeg/mjpeg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mjpeg_mjpeg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mjpeg_mjpeg.c$(PreprocessSuffix): ../../arch/AX3292/mjpeg/mjpeg.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mjpeg_mjpeg.c$(PreprocessSuffix) "../../arch/AX3292/mjpeg/mjpeg.c"

$(IntermediateDirectory)/sdc_sdc.c$(ObjectSuffix): ../../arch/AX3292/sdc/sdc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/sdc/sdc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdc_sdc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdc_sdc.c$(PreprocessSuffix): ../../arch/AX3292/sdc/sdc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdc_sdc.c$(PreprocessSuffix) "../../arch/AX3292/sdc/sdc.c"

$(IntermediateDirectory)/spi_spi0.c$(ObjectSuffix): ../../arch/AX3292/spi/spi0.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/spi/spi0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/spi_spi0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/spi_spi0.c$(PreprocessSuffix): ../../arch/AX3292/spi/spi0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/spi_spi0.c$(PreprocessSuffix) "../../arch/AX3292/spi/spi0.c"

$(IntermediateDirectory)/startup_BLDRX32.S$(ObjectSuffix): ../../arch/AX3292/startup/BLDRX32.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/BLDRX32.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_BLDRX32.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_BLDRX32.S$(PreprocessSuffix): ../../arch/AX3292/startup/BLDRX32.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_BLDRX32.S$(PreprocessSuffix) "../../arch/AX3292/startup/BLDRX32.S"

$(IntermediateDirectory)/startup_boost.S$(ObjectSuffix): ../../arch/AX3292/startup/boost.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/boost.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_boost.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_boost.S$(PreprocessSuffix): ../../arch/AX3292/startup/boost.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_boost.S$(PreprocessSuffix) "../../arch/AX3292/startup/boost.S"

$(IntermediateDirectory)/startup_boot.S$(ObjectSuffix): ../../arch/AX3292/startup/boot.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/boot.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_boot.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_boot.S$(PreprocessSuffix): ../../arch/AX3292/startup/boot.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_boot.S$(PreprocessSuffix) "../../arch/AX3292/startup/boot.S"

$(IntermediateDirectory)/startup_bootsecs.c$(ObjectSuffix): ../../arch/AX3292/startup/bootsecs.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/bootsecs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_bootsecs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_bootsecs.c$(PreprocessSuffix): ../../arch/AX3292/startup/bootsecs.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_bootsecs.c$(PreprocessSuffix) "../../arch/AX3292/startup/bootsecs.c"

$(IntermediateDirectory)/startup_DBR.S$(ObjectSuffix): ../../arch/AX3292/startup/DBR.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/DBR.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_DBR.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_DBR.S$(PreprocessSuffix): ../../arch/AX3292/startup/DBR.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_DBR.S$(PreprocessSuffix) "../../arch/AX3292/startup/DBR.S"

$(IntermediateDirectory)/startup_MBR.S$(ObjectSuffix): ../../arch/AX3292/startup/MBR.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/MBR.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_MBR.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_MBR.S$(PreprocessSuffix): ../../arch/AX3292/startup/MBR.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_MBR.S$(PreprocessSuffix) "../../arch/AX3292/startup/MBR.S"

$(IntermediateDirectory)/startup_vector.S$(ObjectSuffix): ../../arch/AX3292/startup/vector.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/startup/vector.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup_vector.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup_vector.S$(PreprocessSuffix): ../../arch/AX3292/startup/vector.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup_vector.S$(PreprocessSuffix) "../../arch/AX3292/startup/vector.S"

$(IntermediateDirectory)/sys_sys.c$(ObjectSuffix): ../../arch/AX3292/sys/sys.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/sys/sys.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sys_sys.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sys_sys.c$(PreprocessSuffix): ../../arch/AX3292/sys/sys.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sys_sys.c$(PreprocessSuffix) "../../arch/AX3292/sys/sys.c"

$(IntermediateDirectory)/timer_timer0.c$(ObjectSuffix): ../../arch/AX3292/timer/timer0.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/timer/timer0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/timer_timer0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/timer_timer0.c$(PreprocessSuffix): ../../arch/AX3292/timer/timer0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/timer_timer0.c$(PreprocessSuffix) "../../arch/AX3292/timer/timer0.c"

$(IntermediateDirectory)/uart_uart0.c$(ObjectSuffix): ../../arch/AX3292/uart/uart0.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/uart/uart0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/uart_uart0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/uart_uart0.c$(PreprocessSuffix): ../../arch/AX3292/uart/uart0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/uart_uart0.c$(PreprocessSuffix) "../../arch/AX3292/uart/uart0.c"

$(IntermediateDirectory)/uart_btuart.c$(ObjectSuffix): ../../arch/AX3292/uart/btuart.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/uart/btuart.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/uart_btuart.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/uart_btuart.c$(PreprocessSuffix): ../../arch/AX3292/uart/btuart.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/uart_btuart.c$(PreprocessSuffix) "../../arch/AX3292/uart/btuart.c"

$(IntermediateDirectory)/usb_usb.c$(ObjectSuffix): ../../arch/AX3292/usb/usb.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/usb/usb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_usb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_usb.c$(PreprocessSuffix): ../../arch/AX3292/usb/usb.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_usb.c$(PreprocessSuffix) "../../arch/AX3292/usb/usb.c"

$(IntermediateDirectory)/usb_237a_1.c$(ObjectSuffix): ../../arch/AX3292/usb/237a_1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/usb/237a_1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_237a_1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_237a_1.c$(PreprocessSuffix): ../../arch/AX3292/usb/237a_1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_237a_1.c$(PreprocessSuffix) "../../arch/AX3292/usb/237a_1.c"

$(IntermediateDirectory)/usb_237a_2.c$(ObjectSuffix): ../../arch/AX3292/usb/237a_2.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/usb/237a_2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_237a_2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_237a_2.c$(PreprocessSuffix): ../../arch/AX3292/usb/237a_2.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_237a_2.c$(PreprocessSuffix) "../../arch/AX3292/usb/237a_2.c"

$(IntermediateDirectory)/usb_usb11.c$(ObjectSuffix): ../../arch/AX3292/usb/usb11.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/usb/usb11.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_usb11.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_usb11.c$(PreprocessSuffix): ../../arch/AX3292/usb/usb11.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_usb11.c$(PreprocessSuffix) "../../arch/AX3292/usb/usb11.c"

$(IntermediateDirectory)/usb_jt525.c$(ObjectSuffix): ../../arch/AX3292/usb/jt525.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/usb/jt525.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usb_jt525.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usb_jt525.c$(PreprocessSuffix): ../../arch/AX3292/usb/jt525.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usb_jt525.c$(PreprocessSuffix) "../../arch/AX3292/usb/jt525.c"

$(IntermediateDirectory)/watchdog_watchdog.c$(ObjectSuffix): ../../arch/AX3292/watchdog/watchdog.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/watchdog/watchdog.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/watchdog_watchdog.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/watchdog_watchdog.c$(PreprocessSuffix): ../../arch/AX3292/watchdog/watchdog.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/watchdog_watchdog.c$(PreprocessSuffix) "../../arch/AX3292/watchdog/watchdog.c"

$(IntermediateDirectory)/ax32xx_rtc_ax32xx_rtc.c$(ObjectSuffix): ../../arch/AX3292/ax32xx_rtc/ax32xx_rtc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/ax32xx_rtc/ax32xx_rtc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ax32xx_rtc_ax32xx_rtc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ax32xx_rtc_ax32xx_rtc.c$(PreprocessSuffix): ../../arch/AX3292/ax32xx_rtc/ax32xx_rtc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ax32xx_rtc_ax32xx_rtc.c$(PreprocessSuffix) "../../arch/AX3292/ax32xx_rtc/ax32xx_rtc.c"

$(IntermediateDirectory)/dac_ax32xx_dac.c$(ObjectSuffix): ../../arch/AX3292/dac/ax32xx_dac.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/dac/ax32xx_dac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dac_ax32xx_dac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dac_ax32xx_dac.c$(PreprocessSuffix): ../../arch/AX3292/dac/ax32xx_dac.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dac_ax32xx_dac.c$(PreprocessSuffix) "../../arch/AX3292/dac/ax32xx_dac.c"

$(IntermediateDirectory)/pwm_pwm.c$(ObjectSuffix): ../../arch/AX3292/pwm/pwm.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/pwm/pwm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pwm_pwm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pwm_pwm.c$(PreprocessSuffix): ../../arch/AX3292/pwm/pwm.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pwm_pwm.c$(PreprocessSuffix) "../../arch/AX3292/pwm/pwm.c"

$(IntermediateDirectory)/tminf_ax32xx_tminf.c$(ObjectSuffix): ../../arch/AX3292/tminf/ax32xx_tminf.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/tminf/ax32xx_tminf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tminf_ax32xx_tminf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tminf_ax32xx_tminf.c$(PreprocessSuffix): ../../arch/AX3292/tminf/ax32xx_tminf.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tminf_ax32xx_tminf.c$(PreprocessSuffix) "../../arch/AX3292/tminf/ax32xx_tminf.c"

$(IntermediateDirectory)/src_ax32xx_auadc.c$(ObjectSuffix): ../../arch/AX3292/ax32xx/src/ax32xx_auadc.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/ax32xx/src/ax32xx_auadc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ax32xx_auadc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ax32xx_auadc.c$(PreprocessSuffix): ../../arch/AX3292/ax32xx/src/ax32xx_auadc.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ax32xx_auadc.c$(PreprocessSuffix) "../../arch/AX3292/ax32xx/src/ax32xx_auadc.c"

$(IntermediateDirectory)/src_ax32xx_dma.c$(ObjectSuffix): ../../arch/AX3292/ax32xx/src/ax32xx_dma.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/ax32xx/src/ax32xx_dma.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ax32xx_dma.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ax32xx_dma.c$(PreprocessSuffix): ../../arch/AX3292/ax32xx/src/ax32xx_dma.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ax32xx_dma.c$(PreprocessSuffix) "../../arch/AX3292/ax32xx/src/ax32xx_dma.c"

$(IntermediateDirectory)/src_ax32xx_wdt.c$(ObjectSuffix): ../../arch/AX3292/ax32xx/src/ax32xx_wdt.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/ax32xx/src/ax32xx_wdt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ax32xx_wdt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ax32xx_wdt.c$(PreprocessSuffix): ../../arch/AX3292/ax32xx/src/ax32xx_wdt.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ax32xx_wdt.c$(PreprocessSuffix) "../../arch/AX3292/ax32xx/src/ax32xx_wdt.c"

##
## Clean
##
clean:
	$(RM) -r ./Debug/


