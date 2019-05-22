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

#include "Auxiliary.h"
#include "MessageManager.h"
#include "Configuration.h"

#include "gui/Button.h"
#include "gui/Listbox.h"
#include "gui/GuiPanel.h"

#include "ai/AiMap.h"
#include "ai/AiAgent.h"
#include "ai/Pathfinder.h"
#include "ai/AiContainer.h"

#include "graphics/RenderQuad.h"
#include "graphics/TerrainBrush.h"
#include "graphics/Console.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/UnitSelection.h"
#include "graphics/RenderObject.h"
#include "graphics/Renderer.h"

#include "Terrain.h"

