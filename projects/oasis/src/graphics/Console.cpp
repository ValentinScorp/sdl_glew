#include "../Precompiled.h"

void Console::init(std::shared_ptr<Renderer> renderer, Configuration *cfg) {
    mRenderer = renderer;
    
    glFontProgram = renderer->createProgram("data/" + cfg->getParameter("Console", "fontVertexShader").value, 
                                        "data/" + cfg->getParameter("Console", "fontFragmentShader").value);

    glFontProjectionMatrix = renderer->getParamFromProgram(glFontProgram, "projectionMatrix");
    glm::mat4 projection = glm::ortho(0.0f, cfg->getParameter("Window", "width").toFloat(), 
                                      0.0f, cfg->getParameter("Window", "height").toFloat());

    glUseProgram(glFontProgram);
    glUniformMatrix4fv(glFontProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Unable to init FreeType Library" << std::endl;
    }
    std::string fontPath("fonts/" + cfg->getParameter("Console", "fontFile").value);
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Unable to load font " << fontPath << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, 32);

    
    for (int n = 0; n < face->num_charmaps; n++) {
        FT_CharMap charmap = face->charmaps[n];
        std::cout << "Charmap " << n << ": Platform id: " << charmap->platform_id << ". Encoding id: " << charmap->encoding_id << std::endl;
    }
    std::cout << face->charmap << std::endl;
    std::cout << face->charmap->platform_id << std::endl;
    std::cout << face->charmap->encoding_id << std::endl;
    
    std::cout  << "Yeru " << (FT_ULong)'ы' << std::endl;
    std::cout << FT_Get_Char_Index(face, (FT_ULong)0x044B) << std::endl;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "FreeType Library unable to laod Glyph " << c << std::endl;
            continue;
        }
        GLuint glTexture = mRenderer->createTexture(face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_RED, (GLvoid*)face->glyph->bitmap.buffer);

        Character character = {
            glTexture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<GLchar, Character>(c, character));
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    glFontVbo = mRenderer->createVbo(0, sizeof(GLfloat) * 6 * 4);
    glFontVao = mRenderer->createVao(glFontVbo, 2, 2, 0, 0, sizeof(float));
}

void Console::destroy() {
    // todo destroy char textures
    mRenderer->destroyProgram(glFontProgram);
    mRenderer->destroyBuffer(glFontVao);
    mRenderer->destroyBuffer(glFontVbo);
}

void Console::loadFont(std::string path) {
    
}

void Console::render() {
    //renderAt(100, 300, 1, std::string("Фыі"));
    renderAt(100, 300, 1, std::string("Text"));
}

void Console::renderAt(Uint16 x, Uint16 y, float scale, std::string text) {
    glUseProgram(glFontProgram);
    glUniform3f(glGetUniformLocation(glFontProgram, "textColor"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(glFontVao);
    
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters[*c];
        //Character ch = characters[65];
        
        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
        
        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        
        GLfloat vertices[6][4] = {
            {xpos,      ypos + h,   0.0f, 0.0f },
            {xpos,      ypos,       0.0f, 1.0f },
            {xpos + w,  ypos,       1.0f, 1.0f },
            
            {xpos,      ypos + h,   0.0, 0.0 },
            {xpos + w,  ypos,       1.0, 1.0 },
            {xpos + w,  ypos + h,   1.0, 0.0 },
        };
        
        glBindBuffer(GL_ARRAY_BUFFER, glFontVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(glFontVao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.glTexture);
        glUniform1i(glGetUniformLocation(glFontProgram, "texture0"), 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        
        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Console::attachFps(std::shared_ptr<Uint16> framesPerSecond) {
    fps = framesPerSecond;
}