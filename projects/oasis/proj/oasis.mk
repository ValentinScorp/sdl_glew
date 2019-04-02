##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=oasis
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/valen/Documents/sdl_glew/projects
ProjectPath            :=C:/Users/valen/Documents/sdl_glew/projects/oasis/proj
IntermediateDirectory  :=./../debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=valen
Date                   :=02/04/2019
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="oasis.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../include $(IncludeSwitch)./../../glew/src 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)mingw32 $(LibrarySwitch)glew $(LibrarySwitch)opengl32 $(LibrarySwitch)SDL2main $(LibrarySwitch)SDL2 $(LibrarySwitch)SDL2_image 
ArLibs                 :=  "mingw32" "glew" "opengl32" "SDL2main" "SDL2" "SDL2_image" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)./../lib $(LibraryPathSwitch)./../../glew/debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Model.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AnimateModel.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Camera.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "..\..\.build-debug\glew" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"..\..\.build-debug\glew":
	@$(MakeDirCommand) "..\..\.build-debug"
	@echo stam > "..\..\.build-debug\glew"




MakeIntermediateDirs:
	@$(MakeDirCommand) "./../debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./../debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix): ../src/graphics/Mesh.cpp $(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/Mesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(DependSuffix): ../src/graphics/Mesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(DependSuffix) -MM ../src/graphics/Mesh.cpp

$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(PreprocessSuffix): ../src/graphics/Mesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(PreprocessSuffix) ../src/graphics/Mesh.cpp

$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix): ../src/main.cpp $(IntermediateDirectory)/up_src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_main.cpp$(DependSuffix): ../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_main.cpp$(DependSuffix) -MM ../src/main.cpp

$(IntermediateDirectory)/up_src_main.cpp$(PreprocessSuffix): ../src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_main.cpp$(PreprocessSuffix) ../src/main.cpp

$(IntermediateDirectory)/up_src_Model.cpp$(ObjectSuffix): ../src/Model.cpp $(IntermediateDirectory)/up_src_Model.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/Model.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Model.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Model.cpp$(DependSuffix): ../src/Model.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Model.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Model.cpp$(DependSuffix) -MM ../src/Model.cpp

$(IntermediateDirectory)/up_src_Model.cpp$(PreprocessSuffix): ../src/Model.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Model.cpp$(PreprocessSuffix) ../src/Model.cpp

$(IntermediateDirectory)/up_src_AnimateModel.cpp$(ObjectSuffix): ../src/AnimateModel.cpp $(IntermediateDirectory)/up_src_AnimateModel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/AnimateModel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AnimateModel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AnimateModel.cpp$(DependSuffix): ../src/AnimateModel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AnimateModel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AnimateModel.cpp$(DependSuffix) -MM ../src/AnimateModel.cpp

$(IntermediateDirectory)/up_src_AnimateModel.cpp$(PreprocessSuffix): ../src/AnimateModel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AnimateModel.cpp$(PreprocessSuffix) ../src/AnimateModel.cpp

$(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix): ../src/Configuration.cpp $(IntermediateDirectory)/up_src_Configuration.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/Configuration.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Configuration.cpp$(DependSuffix): ../src/Configuration.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Configuration.cpp$(DependSuffix) -MM ../src/Configuration.cpp

$(IntermediateDirectory)/up_src_Configuration.cpp$(PreprocessSuffix): ../src/Configuration.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Configuration.cpp$(PreprocessSuffix) ../src/Configuration.cpp

$(IntermediateDirectory)/up_src_Camera.cpp$(ObjectSuffix): ../src/Camera.cpp $(IntermediateDirectory)/up_src_Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Camera.cpp$(DependSuffix): ../src/Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Camera.cpp$(DependSuffix) -MM ../src/Camera.cpp

$(IntermediateDirectory)/up_src_Camera.cpp$(PreprocessSuffix): ../src/Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Camera.cpp$(PreprocessSuffix) ../src/Camera.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./../debug/


