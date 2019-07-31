#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/perpendicular.hpp> 

#include <stdio.h>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <map>
#include <locale> 
#include <codecvt>
#include <chrono>

#include "Timer.h"
#include "Auxiliary.h"
#include "MessageManager.h"
#include "IniFile.h"

#include "gui/Button.h"
#include "gui/Listbox.h"
#include "gui/GuiPanel.h"

#include "graphics/RenderQuad.h"
#include "graphics/RenderLine.h"
#include "graphics/TerrainBrush.h"
#include "graphics/Console.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/UnitSelection.h"
#include "graphics/RenderObject.h"
#include "graphics/Renderer.h"

#include "ai/AiMap.h"
#include "ai/AiWall.h"
#include "ai/AiObstacle.h"
#include "ai/AiAgent.h"
#include "ai/Pathfinder.h"
#include "ai/AiContainer.h"

#include "world/Terrain.h"
#include "world/WorldObject.h"
#include "world/World.h"




