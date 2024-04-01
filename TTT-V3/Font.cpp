#include "Font.h"

Font::Font(const char* path, unsigned int fontH, float width, float height)
{
    m_width = width;
    m_height = height;
    m_fontHeight = fontH;
    m_projection = glm::ortho(0.0f, width, 0.0f, height);

    // populate characters map
    InitCharacters(path);

    // set up VBO and VAO for drawing characters
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // 4x6 for 6 verts with 4 floats each | dynamic draw because we plan on chaning the verts often
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * Automatically populates the character map
 * 
 * @param path: path to truetype file
 */
void Font::InitCharacters(const char* path)
{
    // init freetype library and error check
    FT_Library ftLib;
    if (FT_Init_FreeType(&ftLib))
    {
        std::cout << "ERROR: Could not init FreeType Lib" << std::endl;
        return;
    }
    // init font face and error check
    FT_Face fontFace;
    if (FT_New_Face(ftLib, path, 0, &fontFace))
    {
        std::cout << "ERROR: Failed to load font (" << path << ')' << std::endl;
        return;
    }
    // Set font size for face (setting width to 0 so it can auto calculate)
    FT_Set_Pixel_Sizes(fontFace, 0, m_fontHeight);

    // Load the characters map
    for (unsigned char c = 0; c < CHAR_N; ++c)
    {
        // load current glyph
        if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR: Failed to load glyph" << std::endl;
            continue;
        }
        // create opengl texture
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_SRGB,
            fontFace->glyph->bitmap.width,
            fontFace->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            fontFace->glyph->bitmap.buffer
        );
        // set scaling mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // set wrap mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // store character in map
        Character character =
        {
            tex,
            glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
            glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
            fontFace->glyph->advance.x
        };
        m_charactersMap.insert(std::pair<char, Character>(c, character));
    }
    // free up memory from freetype
    FT_Done_Face(fontFace);
    FT_Done_FreeType(ftLib);
}

// updates width, height, and projection variables
void Font::UpdateWH(float width, float height)
{
    m_width = width;
    m_height = height;
    m_projection = glm::ortho(0.0f, width, 0.0f, height);
}

/**
 * Draws given text to screen
 * 
 * @param shader: shaderprogram to draw with
 * @param text: text to draw to screen
 * @param x: x screen position
 * @param y: y screen position
 * @param scale: value to scale by
 * @param color: vec3 containing rgb color for font
 */
void Font::DrawText(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // activate shader and set uniforms
    shader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader->ID(), "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniform3f(glGetUniformLocation(shader->ID(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    // bind VAO
    glBindVertexArray(m_VAO);
    // iterate through string
    for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
    {
        Character ch = m_charactersMap[*c];

        float xpos = x + ch.Bearing.x * scale;
        // have to calculate position based on bearing and size becuase of chars that dip below
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float width = ch.Size.x * scale;
        float height = ch.Size.y * scale;

        // update VBO verts
        float vertices[6][4] =
        {   // triangle 1
            {xpos        , ypos + height, 0.0f, 0.0f},
            {xpos        , ypos         , 0.0f, 1.0f},
            {xpos + width, ypos         , 1.0f, 1.0f },
            // triangle 2
            {xpos        , ypos + height, 0.0f, 0.0f},
            {xpos + width, ypos         , 1.0f, 1.0f},
            {xpos + width, ypos + height, 1.0f, 0.0f },
        };
        // render glyph texture
        glBindTexture(GL_TEXTURE_2D, ch.TexID);
        // update VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render triangles
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // advance cursor for next glyph (in 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pix
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

Font::~Font()
{
}
