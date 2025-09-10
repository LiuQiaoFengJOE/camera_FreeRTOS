##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=_build-appoplat-atbm
ConfigurationName      :=Debug
WorkspacePath          := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build"
ProjectPath            := "E:\SVN-Demo-lib\Ax3292\AIcam\DHZJ-E01\firmware\sdk\build"
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
OutputFile             :=
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="_build-appoplat-atbm.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.7.1/bin/windres.exe
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
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
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.7.1/bin/or1k-elf-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite


Objects=

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d ".build-debug\AX3292-bootfat32" ".build-debug\AX3292-lwip" ".build-debug\AX3292-mcu" ".build-debug\AX3292-os" ".build-debug\AX3292-sdrtune" ".build-debug\AX3292-unzip" ".build-debug\AX3292-lib20" ".build-debug\AX3292-libex" ".build-debug\AX3292-sys" ".build-debug\AX3292_atbm6031" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

".build-debug\AX3292-bootfat32":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-bootfat32"


".build-debug\AX3292-lwip":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-lwip"


".build-debug\AX3292-mcu":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-mcu"


".build-debug\AX3292-os":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-os"


".build-debug\AX3292-sdrtune":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-sdrtune"


".build-debug\AX3292-unzip":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-unzip"


".build-debug\AX3292-lib20":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-lib20"


".build-debug\AX3292-libex":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-libex"


".build-debug\AX3292-sys":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292-sys"


".build-debug\AX3292_atbm6031":
	@$(MakeDirCommand) ".build-debug"
	@echo stam > ".build-debug\AX3292_atbm6031"




$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
##
## Clean
##
clean:
	$(RM) -r ./Debug/


