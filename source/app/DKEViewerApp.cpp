#include <app/DKEViewerApp.h>
#include <app/DKEViewerRenderManager.h>
#include <graphics_gl/GLRenderManager.h>
#include <functional>
#include <iostream>

namespace dke {

DKEViewerApp* DKEViewerApp::m_instance = nullptr;

//-------------------------------------------------------------------------------------------------

DKEViewerApp::DKEViewerApp()
    : m_window(nullptr)
    , m_appController(new dke::DKEViewerRenderManager)
{
}

//-------------------------------------------------------------------------------------------------

DKEViewerApp::~DKEViewerApp()
{
}

//-------------------------------------------------------------------------------------------------

DKEViewerApp* DKEViewerApp::instance()
{
    if (!m_instance)
        m_instance = new DKEViewerApp;

    return m_instance;
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::registerCallbacks()
{
//
//class MyClass {
//public:
//    void (*callback)(const char *);
//    int value; // Added so MyClass instances appear in disassembly
//
//    void myCallback(const char *data) const {
//        /*value = */puts(data);
//}
//
//constexpr MyClass(void (*cb)(const char*)) :
//    callback(cb), value(0) {}
//};
//
//template<auto& inst, auto func>
//constexpr auto make_member_callback()
//{
//    return [](auto... args) { (inst.*func)(args...); };
//}
//constexpr static MyClass mc{
//    make_member_callback<mc, &MyClass::myCallback>()
//};
//mc.callback("hi");

    // Tried using above block to ease create callback
    // but make_member_callback does not compile in VS2022. Need to investigate.

    m_keyCB = [](GLFWwindow* win, int key, int scancode, int action, int mods) { DKEViewerApp::instance()->keyCallback(win, key, scancode, action, mods); };
    glfwSetKeyCallback(m_window, m_keyCB);
    m_cursorposCB = [](GLFWwindow* window, double xpos, double ypos) { DKEViewerApp::instance()->mousePosCallback(window, xpos, ypos); };
    glfwSetCursorPosCallback(m_window, m_cursorposCB);
    m_mousebuttonCB = [](GLFWwindow* window, int button, int action, int mods) { DKEViewerApp::instance()->mouseButtonCallback(window, button, action, mods); };
    glfwSetMouseButtonCallback(m_window, m_mousebuttonCB);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::initialize()
{
	glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    m_window = glfwCreateWindow(1024, 768, "DKE Viewer", NULL, NULL);
    if (!m_window)
        glfwTerminate();

    registerCallbacks();

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (glewInit() != GL_NO_ERROR) {
        glfwTerminate();
    }

    m_appController.initialize();
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::run()
{
    if (!m_window)
        return;

    while (!glfwWindowShouldClose(m_window))
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        // TODO: implement onResize event.
        glViewport(0, 0, width, height);

        m_appController.update();
        m_appController.render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::terminate()
{
    if (!m_window)
        return;

    glfwDestroyWindow(m_window);
	glfwTerminate();
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::errorCallback(int errorCode, const char* description)
{
    std::cout << "GLFW error: " << errorCode << ", description: " << description << std::endl;
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::keyCallback(GLFWwindow*, int key, int scancode, int action, int mods)
{

}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    m_mouseX = (int32_t)xpos;
    m_mouseY = (int32_t)ypos;

    m_appController.onMouseMove(MouseEventData{ m_mouseX, m_mouseY, 0 });
}

//-------------------------------------------------------------------------------------------------

void DKEViewerApp::mouseButtonCallback(GLFWwindow*, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            m_appController.onMouseLeftDown(MouseEventData{ m_mouseX, m_mouseY, mods });
            break;
        case GLFW_MOUSE_BUTTON_2:
            m_appController.onMouseRightDown(MouseEventData{ m_mouseX, m_mouseY, mods });
            break;
        case GLFW_MOUSE_BUTTON_3:
            m_appController.onMouseMiddleDown(MouseEventData{ m_mouseX, m_mouseY, mods });
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            m_appController.onMouseLeftUp(MouseEventData{ m_mouseX, m_mouseY, mods });
            break;
        case GLFW_MOUSE_BUTTON_2:
            m_appController.onMouseRightUp(MouseEventData{ m_mouseX, m_mouseY, mods });
            break;
        case GLFW_MOUSE_BUTTON_3:
            m_appController.onMouseMiddleUp(MouseEventData{ m_mouseX, m_mouseY, mods });
            break;
        default:
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------

} // namespace dke
