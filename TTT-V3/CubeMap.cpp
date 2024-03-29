#include "CubeMap.h"

CubeMap::CubeMap(std::vector<std::string> *paths, GLuint slot)
{
    m_unit = slot;
    // create cubemap texture
    glGenTextures(1, &m_ID);
    // bind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
    // set scaling mode
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // set texture to clamp to edge to prevent seems from showing
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // go through cube textures and read them in as textures
    for (unsigned int i = 0; i < 6; ++i)
    {
        stbi_set_flip_vertically_on_load(false);
        int width, height, colCh;
        unsigned char* data = stbi_load(paths->at(i).c_str(), &width, &height, &colCh, 0);
        // check number of color channels
        GLenum format = 0;
        if (colCh == 3)
        {
            format = GL_RGB;
        }
        else if (colCh == 4)
        {
            format = GL_RGBA;
        }
        else
        {
            std::cout << "ERROR: INVALID COLOR CHANNELS IN CUBEMAP TEXTURE" << std::endl;
        }
        // set texture from image
        if (data)
        {
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                format,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << stbi_failure_reason();
            throw new std::exception(stbi_failure_reason());
            stbi_image_free(data);
        }
    }
}

void CubeMap::Bind()
{
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void CubeMap::Unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{
    glDeleteTextures(1, &m_ID);
}
