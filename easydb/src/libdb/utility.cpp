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