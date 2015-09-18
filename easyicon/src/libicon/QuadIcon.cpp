#include "QuadIcon.h"

namespace eicon
{

QuadIcon::QuadIcon()
{
	memset(m_src, 0, sizeof(m_src));
	memset(m_screen, 0, sizeof(m_screen));
}

QuadIcon::QuadIcon(d2d::Image* img, const d2d::Vector* src, 
				   const d2d::Vector* screen)
	: Icon(img)
{
	memcpy(m_src, src, sizeof(m_src));
	memcpy(m_screen, screen, sizeof(m_screen));
}

void QuadIcon::LoadFromFile(const Json::Value& value)
{	
	for (int i = 0; i < 4; ++i) {
		m_src[i].x = value["src"][i]["x"].asDouble();
		m_src[i].y = value["src"][i]["y"].asDouble();
		m_screen[i].x = value["screen"][i]["x"].asDouble();
		m_screen[i].y = value["screen"][i]["y"].asDouble();
	}
}

void QuadIcon::StoreToFile(Json::Value& value) const
{
	for (int i = 0; i < 4; ++i) {
		value["src"][i]["x"] = m_src[i].x;
		value["src"][i]["y"] = m_src[i].y;
		value["screen"][i]["x"] = m_screen[i].x;
		value["screen"][i]["y"] = m_screen[i].y;
	}
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

void QuadIcon::SetScreen(const d2d::Vector* screen)
{
	float w = m_img->GetClippedWidth(),
		h = m_img->GetClippedHeight();
	memcpy(m_screen, screen, sizeof(m_screen));
	for (int i = 0; i < 4; ++i) {
		m_src[i].x = m_screen[i].x / w + 0.5f;
		m_src[i].y = m_screen[i].y / h + 0.5f;
	}
}

void QuadIcon::AfterSetImage()
{
	m_src[0].set(0, 0);
	m_src[1].set(0, 1);
	m_src[2].set(1, 1);
	m_src[3].set(1, 0);

	float hw = m_img->GetClippedWidth() * 0.5f,
		hh = m_img->GetClippedHeight() * 0.5f;
	m_screen[0].set(-hw, -hh);
	m_screen[1].set(-hw,  hh);
	m_screen[2].set( hw,  hh);
	m_screen[3].set( hw, -hh);	
}

}