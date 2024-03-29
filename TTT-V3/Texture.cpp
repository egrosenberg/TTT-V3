#include "Texture.h"

Texture::Texture(const char* fname, const char *texType, GLuint slot)
{
    m_type = texType;
    std::cout << slot << std::endl;

    // Load image
    stbi_set_flip_vertically_on_load(true);
    int widthImg, heightImg, numColCh;
    unsigned char* bytes = stbi_load(fname, &widthImg, &heightImg, &numColCh, 0);
    if (!bytes)
    {
        std::cout << stbi_failure_reason();
        throw new std::exception(stbi_failure_reason());
    }
    // Create and generate our texture
    glGenTextures(1, &m_ID);
    // activate texture unit
    glActiveTexture(GL_TEXTURE0 + slot);
    m_unit = slot;
    // bind texture
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // set scaling mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // set repeat mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // check number of color channels and create texture
    if (numColCh == 4)
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_SRGB_ALPHA,
            widthImg,
            heightImg,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else if (numColCh == 3)
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_SRGB,
            widthImg,
            heightImg,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else if (numColCh == 1)
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_SRGB,
            widthImg,
            heightImg,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else
    {
        throw std::invalid_argument("Texture type recognition failed: Invalid number of color channels");
    }

    // Generate Mimaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // delete img data and unbind texture
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader* shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader->ID(), uniform);
    shader->Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}