//-------------------------------------------------------------------------------------------------
#ifndef DKE_ViewerApp_h
#define DKE_ViewerApp_h
//-------------------------------------------------------------------------------------------------
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <app/DKEViewerAppController.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

class DKEViewerApp 
{
private:
	DKEViewerApp();
	~DKEViewerApp();

	void registerCallbacks();

public:
	static DKEViewerApp* instance();

	void initialize();
	void run();
	void terminate();

	void keyCallback(GLFWwindow*, int key, int scancode, int action, int mods);
	void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow*, int button, int action, int mods);
	void errorCallback(int errorCode, const char* description);

private:
	GLFWwindow* m_window;
	static DKEViewerApp* m_instance;

	GLFWkeyfun m_keyCB;
	GLFWcursorposfun m_cursorposCB;
	GLFWmousebuttonfun m_mousebuttonCB;

	DKEViewerAppController m_appController;
	int32_t m_mouseX, m_mouseY;
};

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_ViewerApp_h
