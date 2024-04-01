#include "Terminal.h"

Terminal *Terminal::m_singletonInstance = NULL;

// gets and returns singleton instance ONLY if it already exists
Terminal *Terminal::GetSingleton()
{
    if (m_singletonInstance != NULL)
    {
        return m_singletonInstance;
    }
    return NULL;
}
/**
 * Gets and returns singleton instance if it exsists
 * otherwise, creates new singleton instance with given values
 * 
 * @param window: glfw window object that the terminal will be used in
 * @param nrows: number of rows the terminal will keep in history
 * @param padding: padding (in pixels) from left side of screen
 * @param lineSpacing: line height as a percentage of font size
 * @param blinkInterval: percentage of second to show cursor
 * @param font: font object to use to draw terminal
 * @param color: vec3 containing rgb values for font color
 */
Terminal *Terminal::GetSingleton(GLFWwindow* window, unsigned int nrows, unsigned int padding, float lineSpacing, float blinkInterval, Font* font, glm::vec3 color)
{
    if (m_singletonInstance != NULL)
    {
        return m_singletonInstance;
    }
    m_singletonInstance = new Terminal(window, nrows, padding, lineSpacing, blinkInterval, font, color);
    return m_singletonInstance;
}

/**
 * Constructor
 * 
 * @param window: glfw window object that the terminal will be used in
 * @param nrows: number of rows the terminal will keep in history
 * @param padding: padding (in pixels) from left side of screen
 * @param lineSpacing: line height as a percentage of font size
 * @param blinkInterval: percentage of second to show cursor
 * @param font: font object to use to draw terminal
 * @param color: vec3 containing rgb values for font color
 */
Terminal::Terminal(GLFWwindow *window, unsigned int nrows, unsigned int padding, float lineSpacing, float blinkInterval, Font *font, glm::vec3 color)
{
    m_active = false;
    m_numRows = nrows;
    m_padding = padding;

    // calculate height of a line of text
    m_lineHeight = font->Height() * lineSpacing;
    // calculate coords to draw terminal at
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    m_yPos = h - (m_lineHeight * m_numRows);

    m_blinkInterval = blinkInterval;
    m_input = "";
    m_history = new std::list<std::string>();
    m_commands = new std::vector<std::pair<std::string, std::function<void(void*)>>>();

    m_font = font;
    m_color = color;

    glfwSetCharCallback(window, staticCharCallback);
    glfwSetKeyCallback(window, staticKeyCallback);
}

/**
 * Calls non-static character callback
 *
 * @param window: glfw window object that called the callback
 * @param codepoint: unicode codepoint value from text input
 */
void Terminal::staticCharCallback(GLFWwindow* window, unsigned int codepoint)
{
    Terminal *singleton = GetSingleton();
    // check to make sure singleton exiists
    if (singleton != NULL)
    {
        return singleton->CharCallback(window, codepoint);
    }
    std::cout << "ERROR: SINGLETON TERMINAL DOES NOT EXIST" << std::endl;
}
/**
 * Calls non-static character callback
 *
 * @param window: glfw windowobject that called the function
 * @param key: key value that triggered the callback
 * @param scancode: scancode for key that triggered the callback
 * @param action: GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods: bit field describing which modifier keys were held
 */
void Terminal::staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Terminal *singleton = GetSingleton();
    // check to make sure singleton exiists
    if (singleton != NULL)
    {
        return singleton->KeyCallback(window, key, scancode, action, mods);
    }
    std::cout << "ERROR: SINGLETON TERMINAL DOES NOT EXIST" << std::endl;
}

/**
 * Handles window text input as terminal input
 * 
 * @param window: glfw window object that called the function
 * @param codepoint: unicode codepoint value from text input
 */
void Terminal::CharCallback(GLFWwindow *window, unsigned int codepoint)
{
    // skip callback if inactive
    if (!m_active)
    {
        return;
    }
    // append text to input string
    m_input += (char)codepoint;
}
/**
 * Handles all misc. keypresses related to terminal (backspace, enter, ect.)
 * 
 * @param window: glfw windowobject that called the function
 * @param key: key value that triggered the callback
 * @param scancode: scancode for key that triggered the callback
 * @param action: GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods: bit field describing which modifier keys were held
 */
void Terminal::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // only care about enter key first as it is the only way to activate the terminal
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        if (!m_active)
        {
            m_active = true;
        }
        else
        {
            if (m_input.size() > 0)
            {
                m_history->push_front(m_input);
                if (m_history->size() > m_numRows)
                {
                    m_history->pop_back();
                }
            }
            m_input = "";
            m_active = false;
        }
    }
    // if terminal is innactive ignore all other input
    if (!m_active)
    {
        return;
    }
    // handle backspace / delete
    if ((key == GLFW_KEY_BACKSPACE || key == GLFW_KEY_DELETE) && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (m_input.size() > 0)
        {
            m_input.pop_back();
        }
    }
    // allow escape of terminal via escape key
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        // erase current input progress and deactivate terminal
        m_input = "";
        m_active = false;
    }
}

void Terminal::BindFunction(std::string, std::function<void(void*)>)
{

}
/**
 * Draws all text in history + current input line
 * 
 * @param shader: shader program to draw with
 * @param crntTime: current window runtime (used for blinking effect)
 */
void Terminal::Draw(Shader *shader, float crntTime)
{
    int i = 0;
    for (std::list<std::string>::iterator text = m_history->begin(); text != m_history->end(); ++text, ++i)
    {
        m_font->DrawText(shader, *text, m_padding, m_yPos + m_lineHeight * i, 1.0f, m_color);
    }
    std::string typing = m_input;
    if (m_active && (crntTime - std::floor(crntTime)) < m_blinkInterval)
    {
        typing += '_';
    }
    m_font->DrawText(shader, typing, m_padding, m_yPos - m_lineHeight, 1.0f, m_color);
}
/**
 * Adds any string to terminal history
 * useful for logging info during runtime
 * 
 * @param text: text to log
 */
void Terminal::Log(std::string text)
{
    m_history->push_front(text);
    if (m_history->size() > m_numRows)
    {
        m_history->pop_back();
    }
}

Terminal::~Terminal()
{
    delete m_history;
    delete m_font;
}
