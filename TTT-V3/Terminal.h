#ifndef TERMINAL_H
#define TERMINAL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <functional>
#include <string>
#include <list>
#include <vector>
#include <utility>
#include <iostream>

#include "Shader.h"
#include "Font.h"

class Terminal
{
private:
    static Terminal *m_singletonInstance;
    bool m_active;
    unsigned int m_numRows;
    unsigned int m_padding;
    float m_lineHeight;
    float m_yPos;
    float m_blinkInterval;
    std::string m_input;
    std::list<std::string> *m_history;
    std::vector<std::tuple<std::string, TTT::TTTenum, std::function<void(void*)>>> *m_commands;
    Font *m_font;
    glm::vec3 m_color;

    static void staticCharCallback(GLFWwindow *window, unsigned int codepoint);
    static void staticKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void CharCallback(GLFWwindow* window, unsigned int codepoint);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    static Terminal *GetSingleton();
    static Terminal *GetSingleton(GLFWwindow* window, unsigned int nrows, unsigned int padding, float lineSpacing, float blinkInterval, Font* font, glm::vec3 color);
    Terminal(GLFWwindow *window, unsigned int nrows, unsigned int padding, float lineSpacing, float blinkInterval, Font *font, glm::vec3 color);

    void BindFunction(std::string name, TTT::TTTenum type, std::function<void(void*)> function);
    void Draw(Shader *shader, float crntTime);
    void Log(std::string text);

    bool Active() { return m_active; }

    virtual ~Terminal();
};

#endif