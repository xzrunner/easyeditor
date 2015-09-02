#include "QuadIcon.h"

namespace eicon
{

QuadIcon::QuadIcon()
{
}

QuadIcon::QuadIcon(d2d::Image* img, const d2d::Vector* src, 
				   const d2d::Vector* screen)
	: Icon(img)
{
	memcpy(m_src, src, sizeof(d2d::Vector) * 4);
	memcpy(m_screen, screen, sizeof(d2d::Vector) * 4);

	m_canvas_region.xMin = m_canvas_region.yMin = 0;
	m_canvas_region.xMax = m_canvas_region.yMax = 1;
}

void QuadIcon::LoadFromFile(const Json::Value& value)
{	
}

void QuadIcon::StoreToFile(Json::Value& value) const
{
}

void QuadIcon::GetBound(float process, d2d::Vector bound[4]) const
{
	memcpy(bound, m_src, sizeof(d2d::Vector) * 4);
}

void QuadIcon::Draw(const d2d::Matrix& mt, float process) const
{
	if (!m_img) {
		return;
	}

	d2d::Vector vertices[4];
	for (int i = 0; i < 4; ++i) {
		vertices[i] = d2d::Math::transVector(m_screen[i], mt);
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vertices, m_src, m_img->GetTexID());
}

d2d::Rect QuadIcon::GetRegion(float process) const
{
	d2d::Rect ret;

	for (int i = 0; i < 4; ++i) {
		float x = m_screen[i].x,
			y = m_screen[i].y;
		if (x < ret.xMin) ret.xMin = x;
		if (x > ret.xMax) ret.xMax = x;
		if (y < ret.yMin) ret.yMin = y;
		if (y > ret.yMax) ret.yMax = y;
	}

	return ret;
}

}