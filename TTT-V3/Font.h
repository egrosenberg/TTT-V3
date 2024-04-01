#ifndef FONT_H
#define FONT_H

#include "Shader.h"
#include "TTT.h"
#include <glm/ext.hpp>
#include <iostream>
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H  // init freetype

#define CHAR_N 128

// struct to handle font characters
struct Character
{
    GLuint TexID;           // ID of the glyph
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to top-left of glyph 
    unsigned int Advance;   // Offset to advance to the next glyph
};

class Font
{
private:
    std::map<char,Character> m_charactersMap;
    
    GLuint m_VAO;
    GLuint m_VBO;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_fontHeight;
    glm::mat4 m_projection;

    void InitCharacters(const char *path);
public:   
    Font(const char* path, unsigned int fontH, float width = (float)WIN_WIDTH, float height = (float)WIN_HEIGHT);

    void UpdateWH(float width, float height);
    void DrawText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);

    unsigned int Height() { return m_fontHeight; }

    virtual ~Font();
};

#endif