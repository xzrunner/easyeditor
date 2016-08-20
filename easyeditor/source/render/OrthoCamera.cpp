#include "OrthoCamera.h"
#include "RenderContextStack.h"

#include <sprite2/RenderCtxStack.h>

namespace ee
{

OrthoCamera::OrthoCamera(float scale/* = 1.0f*/)
	: m_scale(scale)
{
	m_scale = scale;

	UpdateModelView();
}

void OrthoCamera::Reset()
{
	m_center.Set(0, 0);
	m_scale = 1;
	UpdateModelView();
}

sm::vec2 OrthoCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	sm::vec2 proj;
	const int xView = x, yView = height - y;
	proj.x = (xView - (width >> 1)) * m_scale + m_center.x;
	proj.y = (yView - (height >> 1)) * m_scale + m_center.y;
	return proj;
}

sm::vec2 OrthoCamera::TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const
{
	sm::vec2 scr;
	float xView = (proj.x - m_center.x) / m_scale + (width >> 1),
		yView = (proj.y - m_center.y) / m_scale + (height >> 1);
	scr.x = xView;
	scr.y = height - yView;
	return scr;
}

void OrthoCamera::UpdateModelView() const
{
	RenderContextStack::Instance()->SetModelView(-m_center, 1/m_scale);

	s2::RenderCtx* ctx = const_cast<s2::RenderCtx*>(s2::RenderCtxStack::Instance()->Top());
	if (ctx) {
		ctx->mv_offset = - m_center;
		ctx->mv_scale = 1 / m_scale;
	}
}

void OrthoCamera::Translate(const sm::vec2& offset)
{
	m_center += offset * m_scale;
	UpdateModelView();
}

void OrthoCamera::Scale(float scale, int x, int y, int width, int height)
{
	float new_scale = m_scale * scale;
	m_center.x = (x - (width >> 1)) * m_scale + m_center.x - (x - (width >> 1)) * new_scale;
	m_center.y = (y - (height >> 1)) * m_scale + m_center.y - (y - (height >> 1)) * new_scale;
	m_scale = new_scale;
	UpdateModelView();
}

}