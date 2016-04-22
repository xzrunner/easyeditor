#include "RenderContext.h"
#include "OrthoCamera.h"
#include "Pseudo3DCamera.h"

#include <shaderlab.h>
#include <render/render.h>

#include <stddef.h>

namespace glue
{

SINGLETON_DEFINITION(RenderContext)

RenderContext::RenderContext() 
	: m_ortho_cam(new OrthoCamera())
	, m_p3d_cam(new Pseudo3DCamera())
	, m_width(0)
	, m_height(0)
{
}

RenderContext::~RenderContext() 
{
	delete m_ortho_cam;
	delete m_p3d_cam;
}

void RenderContext::Init()
{

}

void RenderContext::OnSize(int w, int h)
{
	sl::SubjectMVP2::Instance()->NotifyProjection(w, h);

	m_p3d_cam->OnSize(w, h);
 	sl::SubjectMVP3::Instance()->NotifyProjection(m_p3d_cam->GetProjectMat());
}

void RenderContext::SetCamera(float x, float y, float sx, float sy)
{
	sl::SubjectMVP2::Instance()->NotifyModelview(x, y, sx, sy);
	sl::SubjectMVP3::Instance()->NotifyModelview(m_p3d_cam->GetModelViewMat());
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