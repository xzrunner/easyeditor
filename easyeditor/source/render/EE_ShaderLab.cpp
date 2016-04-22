#include "EE_ShaderLab.h"
#include "Exception.h"

#include <shaderlab.h>
#include <render/render.h>

#include <stddef.h>

namespace ee
{

SINGLETON_DEFINITION(ShaderLab)

ShaderLab::ShaderLab() 
{
}

ShaderLab::~ShaderLab() 
{
}

void ShaderLab::Init()
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->CreateContext(4096);
	sl::RenderContext* rc = mgr->GetContext();

	mgr->CreateShader(sl::SHAPE2, new sl::Shape2Shader(rc));
	mgr->CreateShader(sl::SHAPE3, new sl::Shape3Shader(rc));
	mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(rc));
	mgr->CreateShader(sl::SPRITE3, new sl::Sprite3Shader(rc));
	mgr->CreateShader(sl::BLEND, new sl::BlendShader(rc));
	mgr->CreateShader(sl::FILTER, new sl::FilterShader(rc));
	mgr->CreateShader(sl::MODEL3, new sl::Model3Shader(rc));
}

void ShaderLab::Update(float dt)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
	shader->UpdateTime(dt);
}

int ShaderLab::CreateTexture(const uint8_t* data, int width, int height, EE_TEXTURE_FORMAT format)
{
	TEXTURE_FORMAT ej_fmt;
	switch (format)
	{
	case EE_TEXTURE_RGBA8:
		ej_fmt = TEXTURE_RGBA8;
		break;
	case EE_TEXTURE_RGB:
		ej_fmt = TEXTURE_RGB;
		break;
	case EE_TEXTURE_A8:
		ej_fmt = TEXTURE_A8;
		break;
 	default:
 		throw ee::Exception("Unknown format %d", format);
	}

	render* r = sl::ShaderMgr::Instance()->GetContext()->GetEJRender();
	RID id = render_texture_create(r, width, height, ej_fmt, TEXTURE_2D, 0);
	render_texture_update(r, id, width, height, data, 0, 0);
	return id;
}

void ShaderLab::ReleaseTexture(int id)
{
	render* r = sl::ShaderMgr::Instance()->GetContext()->GetEJRender();
	render_release(r, TEXTURE, id);
}

void ShaderLab::UpdateTexture(const uint8_t* data, int x, int y, int w, int h, int id)
{
	render* r = sl::ShaderMgr::Instance()->GetContext()->GetEJRender();
	render_texture_subupdate(r, id, data, x, y, w, h);
}

int ShaderLab::GetTexGLID(int tex)
{
	render* r = sl::ShaderMgr::Instance()->GetContext()->GetEJRender();
	return render_query_tex_glid(r, tex);	
}

}