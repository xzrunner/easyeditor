#include "RenderContext.h"
#include "ShaderMgr.h"

namespace d2d
{

RenderContext::RenderContext()
{
	m_mod_scale = 0;
	m_proj_width = m_proj_height = 0;
}

void RenderContext::SetModelView(const Vector& offset, float scale)
{
	m_mod_offset = offset;
	m_mod_scale = scale;

	ShaderMgr::Instance()->SetModelView(offset, scale);
}

void RenderContext::SetProjection(int width, int height)
{
	m_proj_width = width;
	m_proj_height = height;

	ShaderMgr::Instance()->SetProjection(width, height);
}

bool RenderContext::GetModelView(Vector& offset, float& scale) const 
{
	if (m_mod_scale == 0) {
		return false;
	} else {
		offset = m_mod_offset; 
		scale = m_mod_scale;
		return true;
	}
}

bool RenderContext::GetProjection(int& width, int& height) const 
{ 
	if (m_proj_width == 0 || m_proj_height == 0) {
		return false;
	} else {
		width = m_proj_width; 
		height = m_proj_height;
		return true;
	}
}


}