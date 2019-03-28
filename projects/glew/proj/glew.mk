##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=glew
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/valen/Documents/sdl_glew/projects
ProjectPath            :=C:/Users/valen/Documents/sdl_glew/projects/glew/proj
IntermediateDirectory  :=./../debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=valen
Date                   :=28/03/2019
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
ObjectsFileList        :="glew.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)./../src 
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
Objects0=$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_glewinfo.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\Users\valen\Documents\sdl_glew\projects/.build-debug"
	@echo rebuilt > "C:\Users\valen\Documents\sdl_glew\projects/.build-debug/glew"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./../debug"


./../debug:
	@$(MakeDirCommand) "./../debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix): ../src/visualinfo.c $(IntermediateDirectory)/up_src_visualinfo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/glew/src/visualinfo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_visualinfo.c$(DependSuffix): ../src/visualinfo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_visualinfo.c$(DependSuffix) -MM ../src/visualinfo.c

$(IntermediateDirectory)/up_src_visualinfo.c$(PreprocessSuffix): ../src/visualinfo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_visualinfo.c$(PreprocessSuffix) ../src/visualinfo.c

$(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix): ../src/glew.c $(IntermediateDirectory)/up_src_glew.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/glew/src/glew.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_glew.c$(DependSuffix): ../src/glew.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_glew.c$(DependSuffix) -MM ../src/glew.c

$(IntermediateDirectory)/up_src_glew.c$(PreprocessSuffix): ../src/glew.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_glew.c$(PreprocessSuffix) ../src/glew.c

$(IntermediateDirectory)/up_src_glewinfo.c$(ObjectSuffix): ../src/glewinfo.c $(IntermediateDirectory)/up_src_glewinfo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/glew/src/glewinfo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_glewinfo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_glewinfo.c$(DependSuffix): ../src/glewinfo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_glewinfo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_glewinfo.c$(DependSuffix) -MM ../src/glewinfo.c

$(IntermediateDirectory)/up_src_glewinfo.c$(PreprocessSuffix): ../src/glewinfo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_glewinfo.c$(PreprocessSuffix) ../src/glewinfo.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./../debug/


