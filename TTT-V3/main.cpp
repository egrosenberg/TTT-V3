#include "main.h"
#include <stdexcept>

std::string readFile(const char* filename)
{
    // create input stream
    std::ifstream in(filename, std::ios::binary);
    // verify file is open
    if (in)
    {
        // create buffer to store file
        std::string contents;
        // resize string to size of input file
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        // move back to begin of file
        in.seekg(0, std::ios::beg);
        // read file to contents
        in.read(&contents[0], contents.size());
        // close file
        in.close();
        // return file data
        return(contents);
    }
    throw(errno);
}


bool wireframe = false;
bool drawSkybox = false;
bool lightingOnly = false;
bool blitToggle = false;
unsigned int blitBuffer = 0;
float ambient = 0.1f;

// create some lights
std::vector<TTTlight> lights =
{
    {false, POINT_LIGHT, glm::vec4(1.00f, 0.00f, 0.00f, 1.0f), glm::vec3( 10.0f, 10.0f,  10.0f), 0.01f, 0.15f},
    {false, SPOT_LIGHT , glm::vec4(1.00f, 1.00f, 0.00f, 1.0f), glm::vec3( 10.0f, 10.0f, -10.0f), 0.85f, 0.90f},
    {false, POINT_LIGHT, glm::vec4(0.00f, 1.00f, 0.00f, 1.0f), glm::vec3(-10.0f, 10.0f, -10.0f), 0.01f, 0.15f},
    {false, SPOT_LIGHT , glm::vec4(0.00f, 1.00f, 1.00f, 1.0f), glm::vec3( 10.0f, 10.0f,  10.0f), 0.85f, 0.90f},
    {false, POINT_LIGHT, glm::vec4(0.00f, 0.00f, 1.00f, 1.0f), glm::vec3( 10.0f, 10.0f, -10.0f), 0.01f, 0.15f},
    {false, SPOT_LIGHT , glm::vec4(1.00f, 0.00f, 1.00f, 1.0f), glm::vec3(-10.0f, 10.0f,  10.0f), 0.85f, 0.90f},
    {false, DIREC_LIGHT, glm::vec4(0.33f, 0.33f, 0.33f, 1.0f), glm::vec3(-10.0f, 10.0f,  10.0f), 0.85f, 0.90f}
};

std::string toggleWF(void *v)
{
    wireframe = !wireframe;
    std::string onoff = wireframe ? "on" : "off";
    return "wireframe set to: " + onoff;
}
std::string toggleSB(void *v)
{
    drawSkybox = !drawSkybox;
    std::string onoff = drawSkybox ? "on" : "off";
    return "skybox set to: " + onoff;
}
std::string toggleLO(void* v)
{
    lightingOnly = !lightingOnly;
    std::string onoff = lightingOnly ? "lighting only" : "full color";
    return "lighting set to: " + onoff;
}
std::string toggleBlit(void* v)
{
    blitToggle = !blitToggle;
    std::string onoff = blitToggle ? "on" : "off";
    return "blit_toggle set to: " + onoff;
}
std::string setBlit(void *v)
{
    if (!v)
    {
        return "invalid pointer";
    }
    unsigned int *n = (unsigned int*)v;
    blitBuffer = *n;
    return "now blitting from " + std::to_string(*n);
}
std::string toggleLight(void *v)
{
    if (!v)
    {
        return "invalid pointer";
    }
    unsigned int *n = (unsigned int*)v;
    if (*n >= lights.size())
    {
        return "light out of bounds";
    }
    lights[*n].on = !lights[*n].on;
    return "light " + std::to_string(*n) + " is now " + (lights[*n].on ? "active" : "inactive");
}
std::string setAmbient(void *v)
{
    if (!v)
    {
        return "invalid pointer";
    }
    float *n = (float*)v;
    ambient = *n;
    return "ambient light set to " + std::to_string(*n);
}
std::string setLightColor(void *v)
{
    if (!v)
    {
        return "invalid pointer";
    }
    glm::vec4 *val = (glm::vec4*)v;
    int index = (int)val->x;
    if (index < 0 || index >= lights.size())
    {
        return "invalid light index (out of bounds)";
    }
    lights[index].color = glm::vec4(val->y, val->z, val->w, 1.0f);
    return "light " + std::to_string(index) + " changed.";
}

int main()
{
    // initialize glfw
    glfwInit();

    // Mark OpenGL version in glfw
    // This project is using version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW to use the core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create our window and ensure it opened properly
    GLFWwindow* winMain = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, NAME, NULL, NULL);
    if (!winMain)
    {
        std::cerr << "Failed to initalize window, exiting." << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce window to current context
    glfwMakeContextCurrent(winMain);

    // init glad
    gladLoadGL();

    // Define the viewport of the OpenGL window
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    // Disable byte-allignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // create a font
    Font *redhat = new Font("Fonts/RedHatDisplay-Light.ttf", FONT_HEIGHT);

    // Create terminal
    Terminal *terminal = Terminal::GetSingleton(winMain, TERMINAL_ROWS, TERMINAL_PADDING, LINE_SPACING,
        TERMINAL_BLINK_INTERVAL, redhat, glm::vec3(1.0f));
    terminal->Log("TTT-V3 successfully loaded.");
    std::function<TTT_GENERIC_FUNCTION> wireframeFunction = std::bind(&toggleWF, std::placeholders::_1);
    terminal->BindFn("wireframe", wireframeFunction, TTTenum::TTT_VOID);
    std::function<TTT_GENERIC_FUNCTION> skyboxfunction = std::bind(&toggleSB, std::placeholders::_1);
    terminal->BindFn("skybox", skyboxfunction, TTTenum::TTT_VOID);
    std::function<TTT_GENERIC_FUNCTION> lightingOnlyFunction = std::bind(&toggleLO, std::placeholders::_1);
    terminal->BindFn("lighting_only", lightingOnlyFunction, TTTenum::TTT_VOID);
    std::function<TTT_GENERIC_FUNCTION> blitToggleFunction = std::bind(&toggleBlit, std::placeholders::_1);
    terminal->BindFn("blit_toggle", blitToggleFunction, TTTenum::TTT_VOID);
    std::function<TTT_GENERIC_FUNCTION> setBlitFunction = std::bind(&setBlit, std::placeholders::_1);
    terminal->BindFn("blit_value", setBlitFunction, TTTenum::TTT_UINT);
    std::function<TTT_GENERIC_FUNCTION> toggleLightFunction = std::bind(&toggleLight, std::placeholders::_1);
    terminal->BindFn("toggle_light", toggleLightFunction, TTTenum::TTT_UINT);
    std::function<TTT_GENERIC_FUNCTION> setAmbientFunction = std::bind(&setAmbient, std::placeholders::_1);
    terminal->BindFn("set_ambient", setAmbientFunction, TTTenum::TTT_FLOAT);
    std::function<TTT_GENERIC_FUNCTION> setColorFunction = std::bind(&setLightColor, std::placeholders::_1);
    terminal->BindFn("set_color", setColorFunction, TTTenum::TTT_VEC4F);

    // set up shader program for main object
    Shader *wireProgram = new Shader("Shaders/default.vert", "Shaders/wireframe.frag", "Shaders/wireframe.geom");
    // set up shader for framebuffer
    Shader *frameProgram = new Shader("Shaders/framebuffer.vert", "Shaders/framebuffer.frag");
    // set up shader for skybox
    Shader *skyboxProgram = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
    // shader for text
    Shader *textProgram = new Shader("Shaders/font.vert", "Shaders/font.frag");
    // shader for g-buffer geom
    Shader *geomPass = new Shader("Shaders/gbuffer_in.vert", "Shaders/gbuffer_in.frag", "Shaders/gbuffer_in.geom");
    Shader *lightingPass = new Shader("Shaders/gbuffer_out.vert", "Shaders/gbuffer_out.frag");

    // create shadow maps for each of these point lights
    std::vector<Shader*> shadowPrograms;
    for (int i = 0; i < lights.size(); ++i)
    {
        if (lights[i].type == POINT_LIGHT)
        {
            shadowPrograms.push_back(new Shader("Shaders/shadowcubemap.vert", "Shaders/shadowcubemap.frag", "Shaders/shadowcubemap.geom"));
        }
        else
        {
            shadowPrograms.push_back(new Shader("Shaders/shadow.vert", "Shaders/shadow.frag"));
        }
    }


    // def light color
    glm::vec4 lightColor = glm::vec4(1.00f, 1.00f, 1.00f, 1.0f);
    // define position and model for light
    glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
    glm::vec3 lightPosDirec = glm::vec3(0.5f, 0.5f, 0.5f);

    // Activate framebuffer shader and set uniform
    frameProgram->Activate();
    glUniform1i(glGetUniformLocation(frameProgram->ID(), "screenTexture"), 0);
    glUniform1f(glGetUniformLocation(frameProgram->ID(), "gamma"), GAMMA);
    // Activate skybox shader
    skyboxProgram->Activate();
    glUniform1i(glGetUniformLocation(skyboxProgram->ID(), "skybox"), 0);


    // enable depth testing buffer
    glEnable(GL_DEPTH_TEST);
    // set depth function
    glDepthFunc(GL_LESS);

    // enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    // enable blending
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // create camera object
    Camera *mainCamera = new Camera(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // create models object
    Scene *scene = new Scene();
    scene->LoadModel("models/ground/scene.gltf");
    scene->LoadModel("models/trees/scene.gltf");

    // transform the model
    glm::vec3 translation = glm::vec3(-10.0f, 10.0f, -10.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 scale = glm::vec3(0.008f);
    glm::mat4 transform = glm::mat4(1.0f);
    //transform = glm::rotate(transform, glm::radians(-90.0f), rotation);
    transform = glm::scale(transform, scale);
    transform = glm::translate(transform, translation);

    // set transform of model
    //model->SetTransform(transform);
    //trees->SetTransform(transform);
    //cat->SetTransform(transform);

    // set up post proccessing display
    Display* display = new Display(AA_SAMPLES);

    // create an array of strings to hold cubemap faces
    std::vector<std::string> skyboxFaces =
    {
        "Textures/Skybox/px.png",
        "Textures/Skybox/nx.png",
        "Textures/Skybox/py.png",
        "Textures/Skybox/ny.png",
        "Textures/Skybox/pz.png",
        "Textures/Skybox/nz.png",
    };

    // Create skybox
    Skybox *skybox = new Skybox(&skyboxFaces, 0);

    // Create shadow map fbo
    FBO *shadowFBO = new FBO(SHADOW_W, SHADOW_H, TTTenum::TTT_DEPTH_FRAMEBUFFER, TTTenum::TTT_TEXTURE_2D);

    std::vector<Shadowmap*> shadowMaps;
    for (int i = 0; i < lights.size(); ++i)
    {
        Shadowmap *shadowMap = new Shadowmap(lights[i].type, SHADOW_W, SHADOW_H, SHADOW_FARPLANE, lights[i].position, shadowPrograms[i]);
        shadowMaps.push_back(shadowMap);
    }

    // create gbuffer
    Gbuffer *gbuffer = new Gbuffer(WIN_WIDTH, WIN_HEIGHT);

    // g-buffer quad
    GLuint gVAO, gVBO;
    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTS), &SCREEN_VERTS, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    TTTenum lightMode = TTTenum::TTT_POINT_LIGHT;
    bool r_pressed = false;

    // set up variables for FPS tracking
    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff = 0.0;
    unsigned int frameCounter = 0;

    // Main loop
    while (!glfwWindowShouldClose(winMain))
    {
        // calculate fps
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        ++frameCounter;
        if (timeDiff >= 1.0 / 30.0)
        {
            std::string fps = std::to_string((1.0 / timeDiff) * frameCounter);
            std::string ms = std::to_string((timeDiff / frameCounter) * 1000);
            std::string newTitle = NAME + (" - " + fps + "fps / " + ms + "ms");
            glfwSetWindowTitle(winMain, newTitle.c_str());
            prevTime = crntTime;
            frameCounter = 0;
        }

        int nLights = shadowMaps.size();
        float step = (glm::pi<float>() / nLights) * 2;
        float fractime = crntTime * 0.33f;
        for (int i = 0; i < nLights; ++i)
        {
            float offset = fractime + step * i;
            lights[i].position.x = glm::sin(offset) * 15;
            lights[i].position.z = glm::cos(offset) * 15;
            shadowMaps[i]->SetLPos(lights[i].position);
            shadowMaps[i]->SetUniforms(shadowPrograms[i]);
            shadowMaps[i]->Bind();
            scene->DrawModels(shadowPrograms[i], mainCamera);
            shadowMaps[i]->Unbind();
        }
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

        // Clean the back depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // make sure depth testing is enabled
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        // enable face culling
        glEnable(GL_CULL_FACE);

        // Camera control
        if (!terminal->Active())
        {
            mainCamera->Inputs(winMain);
        }
        // update camera matrix
        mainCamera->UpdateMatrix(FOV, 0.1f, 100.0f);
        
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

        // Draw scene
        // bind g-buffer
        //glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        gbuffer->BindFBO();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render to g-buffer
        scene->DrawModels(geomPass, mainCamera);
        //cat->Draw(geomPass, mainCamera);
        // unvind g-buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Do lighting pass
        // bind display buffer before drawing
        display->Bind(WIN_WIDTH, WIN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightingPass->Activate();
        // bind gbuffer as read framebuffer for textures
        gbuffer->BindFBO(GL_READ_FRAMEBUFFER);
        // bind textures
        gbuffer->BindTextures(lightingPass);
        
        // Send the light info to the shader
        glUniform1f(glGetUniformLocation(lightingPass->ID(), "ambientVal"), ambient);
        glUniform1i(glGetUniformLocation(lightingPass->ID(), "lightingOnly"), lightingOnly);
        glUniform1f(glGetUniformLocation(lightingPass->ID(), "farPlane"), SHADOW_FARPLANE);
        glUniform4f(glGetUniformLocation(lightingPass->ID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(lightingPass->ID(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform1i(glGetUniformLocation(lightingPass->ID(), "nLights"), nLights);
        for (int i = 0; i < nLights; ++i)
        {
            glUniform1i( glGetUniformLocation(lightingPass->ID(), ("light[" + std::to_string(i) + "].on").c_str()), lights[i].on);
            glUniform1ui(glGetUniformLocation(lightingPass->ID(), ("light[" + std::to_string(i) + "].type").c_str()), lights[i].type);
            glUniform4f( glGetUniformLocation(lightingPass->ID(), ("light[" + std::to_string(i) + "].color").c_str()), lights[i].color.x, lights[i].color.y, lights[i].color.z, lights[i].color.w);
            glUniform3f( glGetUniformLocation(lightingPass->ID(), ("light[" + std::to_string(i) + "].position").c_str()), lights[i].position.x, lights[i].position.y, lights[i].position.z);
            glUniform1f( glGetUniformLocation(lightingPass->ID(), ("light[" + std::to_string(i) + "].a").c_str()), lights[i].a);
            glUniform1f( glGetUniformLocation(lightingPass->ID(), ("light[" + std::to_string(i) + "].b").c_str()), lights[i].b);
        }

        // set view position
        glm::vec3 campos = mainCamera->GetPos();
        glUniform3f(glGetUniformLocation(lightingPass->ID(), "camPos"), campos.x, campos.y, campos.z);
        mainCamera->Matrix(lightingPass, "camMatrix");
        // Bind the Shadow Map
        for (int i = 0; i < nLights; ++i)
        {
            glActiveTexture(GL_TEXTURE0 + G_BUFFER_DEPTH + i);
            shadowMaps[i]->BindTex();
            if (lights[i].type != POINT_LIGHT)
            {
                shadowMaps[i]->SetUniforms(lightingPass, ("lightProj[" + std::to_string(i) + "]").c_str());
            }
            else
            {
                glm::mat4 temp = glm::mat4(1.0f);
                glUniformMatrix4fv(glGetUniformLocation(lightingPass->ID(), ("lightProj[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(temp));
            }
            glUniform1i(glGetUniformLocation(lightingPass->ID(), ("shadowCubeMap[" + std::to_string(i) + "]").c_str()), G_BUFFER_DEPTH + i);
            glUniform1i(glGetUniformLocation(lightingPass->ID(), ("shadowMap[" + std::to_string(i) + "]").c_str()), G_BUFFER_DEPTH + i);
        }
        
        // bind rect vao
        glBindVertexArray(gVAO);
        // disable depth testing and face culling so we can draw our rect
        glEnable(GL_BLEND_COLOR);
        glDisable(GL_CULL_FACE);
        // draw our rect
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // reenable depth testing and face culling
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        // copy depth buffer buffer from gbuffer to display's depth buffer
        gbuffer->BindFBO(GL_READ_FRAMEBUFFER);
        //glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        display->Bind(WIN_WIDTH, WIN_HEIGHT, GL_DRAW_FRAMEBUFFER);
        // blit contents
        glBlitFramebuffer(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        // draw wireframe if we want
        if (wireframe)
        {
            glEnable(GL_DEPTH_TEST);
            scene->DrawModels(wireProgram, mainCamera);
        }
        // draw skybox
        if (drawSkybox)
        {
            skybox->Draw(skyboxProgram, mainCamera, (float)WIN_WIDTH / WIN_HEIGHT);
        }

        // draw terminal
        terminal->Draw(textProgram, crntTime);

        // unbind display buffer
        display->Unbind();

        // draw display buffer
        display->Draw(frameProgram, winMain);
        // simply blit over from g_buffer if we want
        if (blitToggle)
        {
            gbuffer->BindFBO(GL_READ_FRAMEBUFFER);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); //destination
            glReadBuffer(GL_COLOR_ATTACHMENT0 + blitBuffer);
            glBlitFramebuffer(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
            terminal->Draw(textProgram, crntTime);
        }

        // Swap the back and front buffers
        glfwSwapBuffers(winMain);
        // Trigger all GLFW events
        glfwPollEvents();
    }

    // clean up objects used for shaders and drawing
    delete scene;
    delete wireProgram;
    delete frameProgram;
    delete display;
    delete skybox;
    delete shadowFBO;
    delete redhat;
    delete geomPass;
    delete lightingPass;
    delete gbuffer;

    // close window
    glfwDestroyWindow(winMain);
    // terminate glfw
    glfwTerminate();
    return 0;
}
