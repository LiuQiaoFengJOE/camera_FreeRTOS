##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-bootfat32
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).elf
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AX3292-bootfat32.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  -Map=$(IntermediateDirectory)/$(ProjectName).map -T../../boot/bootfat32/ports/AX3292/AX3292-bootfat32.ld
IncludePath            :=  $(IncludeSwitch). 
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
CFLAGS   :=  -g -Os -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.7.1/bin/or1k-elf-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/bootfat32_fat32.c$(ObjectSuffix) $(IntermediateDirectory)/AX3292_entry.S$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	or1k-elf-objcopy -O binary $(IntermediateDirectory)/$(ProjectName).elf ../$(ProjectName).bin
	or1k-elf-objdump -d $(IntermediateDirectory)/$(ProjectName).elf >$(IntermediateDirectory)/$(ProjectName).lst
	@echo Done

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/bootfat32_fat32.c$(ObjectSuffix): ../../boot/bootfat32/fat32.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/boot/bootfat32/fat32.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bootfat32_fat32.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bootfat32_fat32.c$(PreprocessSuffix): ../../boot/bootfat32/fat32.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bootfat32_fat32.c$(PreprocessSuffix) "../../boot/bootfat32/fat32.c"

$(IntermediateDirectory)/AX3292_entry.S$(ObjectSuffix): ../../boot/bootfat32/ports/AX3292/entry.S 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/boot/bootfat32/ports/AX3292/entry.S" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_entry.S$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_entry.S$(PreprocessSuffix): ../../boot/bootfat32/ports/AX3292/entry.S
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_entry.S$(PreprocessSuffix) "../../boot/bootfat32/ports/AX3292/entry.S"

##
## Clean
##
clean:
	$(RM) -r ./Debug/


