##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=glew
ConfigurationName      :=Debug
WorkspacePath          :=/home/valentin/Documents/sdl_glew/projects
ProjectPath            :=/home/valentin/Documents/sdl_glew/projects/glew/proj
IntermediateDirectory  :=./../debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Valentin
Date                   :=05/24/19
CodeLitePath           :=/home/valentin/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
MakeDirCommand         :=mkdir -p
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
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
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
	@$(MakeDirCommand) "/home/valentin/Documents/sdl_glew/projects/.build-debugwin32"
	@echo rebuilt > "/home/valentin/Documents/sdl_glew/projects/.build-debugwin32/glew"

MakeIntermediateDirs:
	@test -d ./../debug || $(MakeDirCommand) ./../debug


./../debug:
	@test -d ./../debug || $(MakeDirCommand) ./../debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix): ../src/visualinfo.c $(IntermediateDirectory)/up_src_visualinfo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/valentin/Documents/sdl_glew/projects/glew/src/visualinfo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_visualinfo.c$(DependSuffix): ../src/visualinfo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_visualinfo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_visualinfo.c$(DependSuffix) -MM ../src/visualinfo.c

$(IntermediateDirectory)/up_src_visualinfo.c$(PreprocessSuffix): ../src/visualinfo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_visualinfo.c$(PreprocessSuffix) ../src/visualinfo.c

$(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix): ../src/glew.c $(IntermediateDirectory)/up_src_glew.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/valentin/Documents/sdl_glew/projects/glew/src/glew.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_glew.c$(DependSuffix): ../src/glew.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_glew.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_glew.c$(DependSuffix) -MM ../src/glew.c

$(IntermediateDirectory)/up_src_glew.c$(PreprocessSuffix): ../src/glew.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_glew.c$(PreprocessSuffix) ../src/glew.c

$(IntermediateDirectory)/up_src_glewinfo.c$(ObjectSuffix): ../src/glewinfo.c $(IntermediateDirectory)/up_src_glewinfo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/valentin/Documents/sdl_glew/projects/glew/src/glewinfo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_glewinfo.c$(ObjectSuffix) $(IncludePath)
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


