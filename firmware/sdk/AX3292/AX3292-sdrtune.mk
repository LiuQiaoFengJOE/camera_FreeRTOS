##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AX3292-sdrtune
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
ObjectsFileList        :="AX3292-sdrtune.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  -T../../board/AX3292/mvl8801/AX3292-sdrtune.ld -Map=$(IntermediateDirectory)/$(ProjectName).map --gc-sections -( -lAX3292-os -lAX3292-mcu -lc -lgcc -lnosys -)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../inc $(IncludeSwitch)../../arch/AX3292 $(IncludeSwitch)../../board/AX3292/mvl8801 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)C:/MinGW-4.7.1/lib/gcc/or1k-elf/4.9.1/no-delay $(LibraryPathSwitch)C:/MinGW-4.7.1/or1k-elf/lib/no-delay  $(LibraryPathSwitch). $(LibraryPathSwitch)../../sdk 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-4.7.1/bin/or1k-elf-ar.exe rcu
CXX      := C:/MinGW-4.7.1/bin/or1k-elf-g++.exe
CC       := C:/MinGW-4.7.1/bin/or1k-elf-gcc.exe -DOR1K_NEWCPU -mno-delay
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -Os -Wall -mhard-div -ffunction-sections -fdata-sections $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.7.1/bin/or1k-elf-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/sdrtune_step0.c$(ObjectSuffix) $(IntermediateDirectory)/sdrtune_step1.c$(ObjectSuffix) $(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) 



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
	or1k-elf-objdump -S $(IntermediateDirectory)/$(ProjectName).elf >$(IntermediateDirectory)/$(ProjectName).lst
	@echo Done

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/sdrtune_step0.c$(ObjectSuffix): ../../arch/AX3292/sdrtune/step0.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/sdrtune/step0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdrtune_step0.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdrtune_step0.c$(PreprocessSuffix): ../../arch/AX3292/sdrtune/step0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdrtune_step0.c$(PreprocessSuffix) "../../arch/AX3292/sdrtune/step0.c"

$(IntermediateDirectory)/sdrtune_step1.c$(ObjectSuffix): ../../arch/AX3292/sdrtune/step1.c 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/arch/AX3292/sdrtune/step1.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdrtune_step1.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdrtune_step1.c$(PreprocessSuffix): ../../arch/AX3292/sdrtune/step1.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdrtune_step1.c$(PreprocessSuffix) "../../arch/AX3292/sdrtune/step1.c"

$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix): ../../board/AX3292/build-nd.s 
	$(CC) $(SourceSwitch) "E:/SVN-Demo-lib/Ax3292/AIcam/DHZJ-E01/firmware/board/AX3292/build-nd.s" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AX3292_build-nd.s$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AX3292_build-nd.s$(PreprocessSuffix): ../../board/AX3292/build-nd.s
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AX3292_build-nd.s$(PreprocessSuffix) "../../board/AX3292/build-nd.s"

##
## Clean
##
clean:
	$(RM) -r ./Debug/


