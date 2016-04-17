#include "RenderContext.h"
#include "Camera.h"

#include <shaderlab.h>
#include <render/render.h>

#include <stddef.h>

namespace glue
{

SINGLETON_DEFINITION(RenderContext)

RenderContext::RenderContext() 
	: m_width(0)
	, m_height(0)
{
}

RenderContext::~RenderContext() 
{
}

void RenderContext::Init()
{
	m_cam = new Camera();

// 	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
// 	sl_mgr->CreateContext(4096);
// 	sl::RenderContext* sl_rc = sl_mgr->GetContext();
// 	sl_mgr->CreateShader(sl::SPRITE, new sl::Sprite2Shader(sl_rc));
// 	sl_mgr->CreateShader(sl::SHAPE, new sl::ShapeShader(sl_rc));
}

void RenderContext::OnSize(int w, int h)
{
	sl::SubjectMVP2::Instance()->NotifyProjection(w, h);

	ej_cam25_create((float)w / h);
	sl::SubjectMVP3::Instance()->NotifyProjection(ej_cam25_get_proj_mat());
}

void RenderContext::SetCamera(float x, float y, float sx, float sy)
{
	sl::SubjectMVP2::Instance()->NotifyModelview(x, y, sx, sy);	
}

RID RenderContext::CreateTexture(const uint8_t* data, int width, int height, TEXTURE_FORMAT format)
{
	render* r = sl::ShaderMgr::Instance()->GetContext()->GetEJRender();
	RID id = render_texture_create(r, width, height, format, TEXTURE_2D, 0);
	render_texture_update(r, id, width, height, data, 0, 0);
	return id;
}

void RenderContext::ReleaseTexture(RID id)
{
	render* r = sl::ShaderMgr::Instance()->GetContext()->GetEJRender();
	render_release(r, TEXTURE, id);
}

}