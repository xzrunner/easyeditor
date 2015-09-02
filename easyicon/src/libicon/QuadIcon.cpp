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
}

void QuadIcon::Draw(const d2d::Matrix& mt, float process) const
{
	if (!m_img) {
		return;
	}

	d2d::Vector texcoords[4];
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = m_src[i];
		texcoords[i].y = 1 - texcoords[i].y;
	}

	d2d::Vector vertices[4];
	for (int i = 0; i < 4; ++i) {
		vertices[i] = d2d::Math::transVector(m_screen[i], mt);
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vertices, texcoords, m_img->GetTexID());
}

void QuadIcon::LoadFromFile(const Json::Value& value)
{	
}

void QuadIcon::StoreToFile(Json::Value& value) const
{
}

void QuadIcon::GetRegion(float process, d2d::Rect& region) const
{
	for (int i = 0; i < 4; ++i) {
		const d2d::Vector& p = m_screen[i];
		if (p.x < region.xMin) region.xMin = p.x;
		if (p.x > region.xMax) region.xMax = p.x;
		if (p.y < region.yMin) region.yMin = p.y;
		if (p.y > region.yMax) region.yMax = p.y;
	}
}

void QuadIcon::GetTexcoords4(d2d::Vector tex4[4], float process) const
{
}

}