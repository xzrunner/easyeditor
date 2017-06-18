#include "utility.h"

#include <ee/EE_DTex.h>
#include <ee/EE_GTxt.h>
#include <gum/GUM_ShaderLab.h>

#include <gl/glew.h>
#include <glfw.h>

namespace edb
{

int init_gl()
{
	glfwInit();
	if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return -2;
	}

	if (glewInit() != GLEW_OK) {
		return -2;
	}

	gum::ShaderLab::Instance()->Init();
	ee::DTex::Init();
	ee::GTxt::Init();

// 	sl::SubjectMVP2::Instance()->NotifyModelview(0, 0, 1, 1);
// 	sl::SubjectMVP2::Instance()->NotifyProjection(100, 100);

	return 0;
}

}