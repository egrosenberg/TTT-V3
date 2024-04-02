#include "FBO.h"

FBO::FBO(unsigned int width, unsigned int height, TTT::TTTenum fboType, TTT::TTTenum texType, unsigned int samples)
{
    m_TexType = texType;
    m_Type = fboType;
    m_Width = width;
    m_Height = height;
    m_Samples = samples;

    // initalize frame buffer
    glGenFramebuffers(1, &m_ID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    if (texType == TTT::TTT_TEXTURE_CUBEMAP)
    {
        TexGenCubemap();
    }
    else if (fboType == TTT::TTT_FRAMEBUFFER)
    {
        TexGen();
    }
    else if (fboType == TTT::TTT_MULTISAMPLE_FRAMEBUFFER)
    {
        TexGenMultisample();
    }
    else if (fboType == TTT::TTT_DEPTH_FRAMEBUFFER)
    {
        TexGenDepthonly();
    }
    else
    {
        std::cerr << "ERROR IN FBO: INVALID FBO TYPE" << std::endl;
    }
}

void FBO::TexGen()
{
    // create texture
    glGenTextures(1, &m_Tex);
    glBindTexture(GL_TEXTURE_2D, m_Tex);
    // configure texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    // set upscaling mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Prevent edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Tex, 0);
}

void FBO::TexGenMultisample()
{
    glGenTextures(1, &m_Tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Tex);
    // configure texture
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, GL_RGB32F, m_Width, m_Height, GL_TRUE);
    // set upscaling mode
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Prevent edge bleeding
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_Tex, 0);
}

void FBO::TexGenDepthonly()
{
    // generate depth map texture
    glGenTextures(1, &m_Tex);
    glBindTexture(GL_TEXTURE_2D, m_Tex);
    // specify texture is storing depth component
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // set scaling mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // make clamp to border
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // make it so any value outside of shadow map has depth value of 1 so it wont be in shadow
    float clampColor[] = { 1.0f , 1.0f , 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);
    // bind depth buffer attachment to FBO & texture
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Tex, 0);
    // specify we wont use color of the buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    // unbind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::TexGenCubemap()
{
    // generate and bind cubemap texture
    glGenTextures(1, &m_Tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Tex);
    // Init cubemap based on texture type
    if (m_Type == TTT::TTT_DEPTH_FRAMEBUFFER)
    {
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        // set scaling mode
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // make clamp to border
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // bind depth buffer attachment to FBO & texture
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_Tex, 0);
        // specify we wont use color of the buffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        // unbind fbo
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    {
        std::cout << "ERROR IN FBO: INVALID FBO TYPE FOR CUBEMAP" << std::endl;
    }
}

void FBO::Bind(GLenum target)
{
    glBindFramebuffer(target, m_ID);
}

void FBO::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::BindTex()
{
    switch (m_TexType)
    {
        case TTT::TTT_TEXTURE_CUBEMAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_Tex);
        default:
            glBindTexture(GL_TEXTURE_2D, m_Tex);
    }
}

void FBO::UnbindTex()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::CheckStatus(const char* name)
{
    // error checking
    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string error = "";
        switch (fboStatus)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            error = "GL_FRAMEBUFFER_UNDEFINED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            error = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            error = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            error = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            error = "GL_FRAMEBUFFER_UNSUPPORTED";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            error = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            error = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            break;
        default:
            error = "UNDEFINED_ERROR";
        }
        std::cout << "ERROR (FRAMEBUFFER - " << name << "): " << error << std::endl;
    }
}

FBO::~FBO()
{
    glDeleteTextures(1, &m_Tex);
}
