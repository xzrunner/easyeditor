#include "QuadIcon.h"

namespace eicon
{

QuadIcon::QuadIcon()
{
}

QuadIcon::QuadIcon(const d2d::Vector* src, const d2d::Vector* screen)
{
	memcpy(m_src, src, sizeof(d2d::Vector) * 4);
	memcpy(m_screen, screen, sizeof(d2d::Vector) * 4);
}

void QuadIcon::Draw(const d2d::Matrix& mt, float process) const
{
	if (!m_img) {
		return;
	}

	float w = m_img->GetOriginWidth(),
		h = m_img->GetOriginHeight();

	d2d::Vector vertices[4];
	for (int i = 0; i < 4; ++i)
	{
		float x = m_src[i].x * w,
			y = m_src[i].y * h;
		vertices[i] = d2d::Math::transVector(d2d::Vector(x, y), mt);
	}

	d2d::Vector texcoords[4];
	for (int i = 0; i < 4; ++i)
	{
		float x = m_screen[i].x * w / w + 0.5f,
			y = m_screen[i].y * h / h + 0.5f;
		texcoords[i].set(x, y);
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
}

void QuadIcon::GetTexcoords4(d2d::Vector tex4[4], float process) const
{
}

}