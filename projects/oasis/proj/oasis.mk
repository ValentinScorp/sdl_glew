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
Date                   :=18/04/2019
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./../include $(IncludeSwitch)./../../glew/src $(IncludeSwitch)/usr/include/freetype2 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)mingw32 $(LibrarySwitch)glew $(LibrarySwitch)opengl32 $(LibrarySwitch)SDL2main $(LibrarySwitch)SDL2 $(LibrarySwitch)SDL2_image $(LibrarySwitch)freetype 
ArLibs                 :=  "mingw32" "glew" "opengl32" "SDL2main" "SDL2" "SDL2_image" "freetype" 
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
Objects0=$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_graphics_Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_gui_Button.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_MessageManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_graphics_Console.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "..\..\.build-debugwin32\glew" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"..\..\.build-debugwin32\glew":
	@$(MakeDirCommand) "..\..\.build-debugwin32"
	@echo stam > "..\..\.build-debugwin32\glew"




MakeIntermediateDirs:
	@$(MakeDirCommand) "./../debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./../debug"

PreBuild:

# PreCompiled Header
../src/Precompiled.h.gch: ../src/Precompiled.h
	$(CXX) $(SourceSwitch) ../src/Precompiled.h $(PCHCompileFlags)



##
## Objects
##
$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix): ../src/graphics/Mesh.cpp $(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/Mesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(DependSuffix): ../src/graphics/Mesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(DependSuffix) -MM ../src/graphics/Mesh.cpp

$(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(PreprocessSuffix): ../src/graphics/Mesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_Mesh.cpp$(PreprocessSuffix) ../src/graphics/Mesh.cpp

$(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(ObjectSuffix): ../src/graphics/Renderer.cpp $(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/Renderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(DependSuffix): ../src/graphics/Renderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(DependSuffix) -MM ../src/graphics/Renderer.cpp

$(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(PreprocessSuffix): ../src/graphics/Renderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_Renderer.cpp$(PreprocessSuffix) ../src/graphics/Renderer.cpp

$(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(ObjectSuffix): ../src/graphics/RenderObject.cpp $(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/RenderObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(DependSuffix): ../src/graphics/RenderObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(DependSuffix) -MM ../src/graphics/RenderObject.cpp

$(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(PreprocessSuffix): ../src/graphics/RenderObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_RenderObject.cpp$(PreprocessSuffix) ../src/graphics/RenderObject.cpp

$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix): ../src/Terrain.cpp $(IntermediateDirectory)/up_src_Terrain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/Terrain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Terrain.cpp$(DependSuffix): ../src/Terrain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Terrain.cpp$(DependSuffix) -MM ../src/Terrain.cpp

$(IntermediateDirectory)/up_src_Terrain.cpp$(PreprocessSuffix): ../src/Terrain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Terrain.cpp$(PreprocessSuffix) ../src/Terrain.cpp

$(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix): ../src/Configuration.cpp $(IntermediateDirectory)/up_src_Configuration.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/Configuration.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Configuration.cpp$(DependSuffix): ../src/Configuration.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Configuration.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Configuration.cpp$(DependSuffix) -MM ../src/Configuration.cpp

$(IntermediateDirectory)/up_src_Configuration.cpp$(PreprocessSuffix): ../src/Configuration.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Configuration.cpp$(PreprocessSuffix) ../src/Configuration.cpp

$(IntermediateDirectory)/up_src_graphics_Camera.cpp$(ObjectSuffix): ../src/graphics/Camera.cpp $(IntermediateDirectory)/up_src_graphics_Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_Camera.cpp$(DependSuffix): ../src/graphics/Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_Camera.cpp$(DependSuffix) -MM ../src/graphics/Camera.cpp

$(IntermediateDirectory)/up_src_graphics_Camera.cpp$(PreprocessSuffix): ../src/graphics/Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_Camera.cpp$(PreprocessSuffix) ../src/graphics/Camera.cpp

$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix): ../src/main.cpp $(IntermediateDirectory)/up_src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_main.cpp$(DependSuffix): ../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_main.cpp$(DependSuffix) -MM ../src/main.cpp

$(IntermediateDirectory)/up_src_main.cpp$(PreprocessSuffix): ../src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_main.cpp$(PreprocessSuffix) ../src/main.cpp

$(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(ObjectSuffix): ../src/gui/GuiPanel.cpp $(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/gui/GuiPanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(DependSuffix): ../src/gui/GuiPanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(DependSuffix) -MM ../src/gui/GuiPanel.cpp

$(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(PreprocessSuffix): ../src/gui/GuiPanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_gui_GuiPanel.cpp$(PreprocessSuffix) ../src/gui/GuiPanel.cpp

$(IntermediateDirectory)/up_src_gui_Button.cpp$(ObjectSuffix): ../src/gui/Button.cpp $(IntermediateDirectory)/up_src_gui_Button.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/gui/Button.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_gui_Button.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_gui_Button.cpp$(DependSuffix): ../src/gui/Button.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_gui_Button.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_gui_Button.cpp$(DependSuffix) -MM ../src/gui/Button.cpp

$(IntermediateDirectory)/up_src_gui_Button.cpp$(PreprocessSuffix): ../src/gui/Button.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_gui_Button.cpp$(PreprocessSuffix) ../src/gui/Button.cpp

$(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(ObjectSuffix): ../src/graphics/TerrainBrush.cpp $(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/TerrainBrush.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(DependSuffix): ../src/graphics/TerrainBrush.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(DependSuffix) -MM ../src/graphics/TerrainBrush.cpp

$(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(PreprocessSuffix): ../src/graphics/TerrainBrush.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_TerrainBrush.cpp$(PreprocessSuffix) ../src/graphics/TerrainBrush.cpp

$(IntermediateDirectory)/up_src_MessageManager.cpp$(ObjectSuffix): ../src/MessageManager.cpp $(IntermediateDirectory)/up_src_MessageManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/MessageManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MessageManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MessageManager.cpp$(DependSuffix): ../src/MessageManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MessageManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MessageManager.cpp$(DependSuffix) -MM ../src/MessageManager.cpp

$(IntermediateDirectory)/up_src_MessageManager.cpp$(PreprocessSuffix): ../src/MessageManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MessageManager.cpp$(PreprocessSuffix) ../src/MessageManager.cpp

$(IntermediateDirectory)/up_src_graphics_Console.cpp$(ObjectSuffix): ../src/graphics/Console.cpp $(IntermediateDirectory)/up_src_graphics_Console.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/valen/Documents/sdl_glew/projects/oasis/src/graphics/Console.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_graphics_Console.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_graphics_Console.cpp$(DependSuffix): ../src/graphics/Console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_graphics_Console.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_graphics_Console.cpp$(DependSuffix) -MM ../src/graphics/Console.cpp

$(IntermediateDirectory)/up_src_graphics_Console.cpp$(PreprocessSuffix): ../src/graphics/Console.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_graphics_Console.cpp$(PreprocessSuffix) ../src/graphics/Console.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./../debug/
	$(RM) ../src/Precompiled.h.gch


