##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=glew_2_1_0
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/valen/Documents/projects
ProjectPath            :=C:/Users/valen/Documents/projects/glew_2_1_0
IntermediateDirectory  :=./debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=valen
Date                   :=18/03/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
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
ObjectsFileList        :="glew_2_1_0.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/glewinfo.c$(ObjectSuffix) $(IntermediateDirectory)/visualinfo.c$(ObjectSuffix) $(IntermediateDirectory)/glew.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "C:\Users\valen\Documents\projects/.build-debug"
	@echo rebuilt > "C:\Users\valen\Documents\projects/.build-debug/glew_2_1_0"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./debug"


./debug:
	@$(MakeDirCommand) "./debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/glewinfo.c$(ObjectSuffix): glewinfo.c $(IntermediateDirectory)/glewinfo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/valen/Documents/projects/glew_2_1_0/glewinfo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glewinfo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glewinfo.c$(DependSuffix): glewinfo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glewinfo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/glewinfo.c$(DependSuffix) -MM glewinfo.c

$(IntermediateDirectory)/glewinfo.c$(PreprocessSuffix): glewinfo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glewinfo.c$(PreprocessSuffix) glewinfo.c

$(IntermediateDirectory)/visualinfo.c$(ObjectSuffix): visualinfo.c $(IntermediateDirectory)/visualinfo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/valen/Documents/projects/glew_2_1_0/visualinfo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/visualinfo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/visualinfo.c$(DependSuffix): visualinfo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/visualinfo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/visualinfo.c$(DependSuffix) -MM visualinfo.c

$(IntermediateDirectory)/visualinfo.c$(PreprocessSuffix): visualinfo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/visualinfo.c$(PreprocessSuffix) visualinfo.c

$(IntermediateDirectory)/glew.c$(ObjectSuffix): glew.c $(IntermediateDirectory)/glew.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/valen/Documents/projects/glew_2_1_0/glew.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glew.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glew.c$(DependSuffix): glew.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glew.c$(ObjectSuffix) -MF$(IntermediateDirectory)/glew.c$(DependSuffix) -MM glew.c

$(IntermediateDirectory)/glew.c$(PreprocessSuffix): glew.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glew.c$(PreprocessSuffix) glew.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./debug/


