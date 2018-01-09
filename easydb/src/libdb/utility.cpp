#include "utility.h"

#include <ee/EE_DTex.h>
#include <ee/EE_GTxt.h>
#include <gum/ShaderLab.h>
#include <gum/RenderContext.h>

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shape2Shader.h>
#include <shaderlab/Shape3Shader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/Sprite3Shader.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/MaskShader.h>
#include <shaderlab/Model3Shader.h>

#include <gl/glew.h>
#include <glfw3.h>

#include <iostream>

namespace edb
{

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static int prepare_gl_context()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(100, 100, "easydb", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	//// Initialize GLEW to setup the OpenGL Function pointers
	//if (glewInit() != GLEW_OK) {
	//	return -1;
	//}
}
	
int init_gl()
{
	int ret = prepare_gl_context();
	if (ret < 0) {
		return ret;
	}

//	gum::ShaderLab::Instance()->Init();
	{
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
		mgr->SetContext(rc);

		mgr->CreateShader(sl::SHAPE2, new sl::Shape2Shader(rc));
		mgr->CreateShader(sl::SHAPE3, new sl::Shape3Shader(rc));
		mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(rc));
		mgr->CreateShader(sl::SPRITE3, new sl::Sprite3Shader(rc));
		mgr->CreateShader(sl::BLEND, new sl::BlendShader(rc));
		mgr->CreateShader(sl::FILTER, new sl::FilterShader(rc));
		mgr->CreateShader(sl::MASK, new sl::MaskShader(rc));
		mgr->CreateShader(sl::MODEL3, new sl::Model3Shader(rc));
	}

	ee::DTex::Init();
	ee::GTxt::Init();

// 	sl::SubjectMVP2::Instance()->NotifyModelview(0, 0, 1, 1);
// 	sl::SubjectMVP2::Instance()->NotifyProjection(100, 100);

	return 0;
}

}