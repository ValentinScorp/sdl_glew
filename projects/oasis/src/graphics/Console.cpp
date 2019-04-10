#include "../Precompiled.h"

void convertUtf8ToUtf16(std::string &src, std::u16string &dst) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; 
    dst = convert.from_bytes(src);
}

void convertUtf16ToUtf8(std::u16string src, std::string &dst) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert; 
    dst = convert.to_bytes(src);
}

void coutUtf16(std::u16string txt) {
    std::cout << std::hex;
    for (char16_t c: txt) {
        std::cout  << "[" << std::uint_least16_t(c) << "] ";
    }
    std::cout << std::endl;
    std::cout << std::dec;
}

void coutUtf8(std::string &txt) {
    std::cout << std::hex;
    for (char c: txt) {
        std::cout  << "[" << int(static_cast<unsigned char>(c)) << "] ";
    }
    std::cout << std::endl;
    std::cout << std::dec;
}

void Console::init(std::shared_ptr<Renderer> renderer, Configuration *cfg) {
    mRenderer = renderer;
    screenWidth = cfg->getParameter("Window", "width").toFloat();
    screenHeight = cfg->getParameter("Window", "height").toFloat();
    
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    FT_UInt glyphIndex = 0;
    FT_ULong charcode = FT_Get_First_Char(face, &glyphIndex);
    while (glyphIndex) {
        FT_Error error;
        if (error = FT_Load_Char(face, charcode, FT_LOAD_RENDER)) {
            std::cerr << "FreeType Library unable to laod Glyph " << charcode << std::endl;
            std::cerr << "FreeType Library error code " << error << std::endl;
            continue;
        }
        GLuint glTexture = mRenderer->createTexture(face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_RED, (GLvoid*)face->glyph->bitmap.buffer);

        Character character = {
            glTexture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<FT_ULong, Character>(charcode, character));
        
        charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    glFontVbo = mRenderer->createVbo(0, sizeof(GLfloat) * 6 * 4);
    glFontVao = mRenderer->createVao(glFontVbo, 2, 2, 0, 0, 0, sizeof(float));
}

void Console::destroy() {
    // todo destroy char textures
    mRenderer->destroyProgram(glFontProgram);
    mRenderer->destroyBuffer(glFontVao);
    mRenderer->destroyBuffer(glFontVbo);
}

void Console::loadFont(std::string path) {
    
}

void Console::renderFrameParameters() {
    std::string strFt = "Frame time: " + std::to_string((Uint16)(*frameTime)) + " ms";
    std::u16string u16Ft;
    convertUtf8ToUtf16(strFt, u16Ft);
    renderAt(screenWidth - 120, screenHeight - 20, 0.5, u16Ft);
}

void Console::render() {
    renderFrameParameters();
}

void Console::renderAt(Uint16 x, Uint16 y, float scale, std::u16string text) {
    glUseProgram(glFontProgram);
    glUniform3f(glGetUniformLocation(glFontProgram, "textColor"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(glFontVao);
    
    std::u16string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters[*c];
        
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

void Console::attachFrameTime(std::shared_ptr<float> ft) {
    frameTime = ft;
}