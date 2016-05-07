#include "QuadIcon.h"

#include <ee/Math2D.h>
#include <ee/Image.h>

#include <shaderlab.h>

namespace eicon
{

QuadIcon::QuadIcon()
{
	memset(m_src, 0, sizeof(m_src));
	memset(m_screen, 0, sizeof(m_screen));
}

QuadIcon::QuadIcon(ee::Image* img, const ee::Vector* src, 
				   const ee::Vector* screen)
	: Icon(img)
{
	memcpy(m_src, src, sizeof(m_src));
	memcpy(m_screen, screen, sizeof(m_screen));
}

void QuadIcon::LoadFromFile(const Json::Value& value)
{	
	for (int i = 0; i < 4; ++i) {
		m_src[i].x    = static_cast<float>(value["src"][i]["x"].asDouble());
		m_src[i].y    = static_cast<float>(value["src"][i]["y"].asDouble());
		m_screen[i].x = static_cast<float>(value["screen"][i]["x"].asDouble());
		m_screen[i].y = static_cast<float>(value["screen"][i]["y"].asDouble());
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

void QuadIcon::GetBound(float process, ee::Vector bound[4]) const
{
	memcpy(bound, m_src, sizeof(ee::Vector) * 4);
}

void QuadIcon::Draw(const sm::mat4& mt, float process) const
{
	if (!m_img) {
		return;
	}

	ee::Vector vertices[4];
	for (int i = 0; i < 4; ++i) {
		vertices[i] = ee::Math2D::TransVector(m_screen[i], mt);
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &m_src[0].x, m_img->GetTexID());
}

ee::Rect QuadIcon::GetRegion(float process) const
{
	ee::Rect ret;

	for (int i = 0; i < 4; ++i) {
		float x = m_screen[i].x,
			y = m_screen[i].y;
		if (x < ret.xmin) ret.xmin = x;
		if (x > ret.xmax) ret.xmax = x;
		if (y < ret.ymin) ret.ymin = y;
		if (y > ret.ymax) ret.ymax = y;
	}

	return ret;
}

void QuadIcon::SetScreen(const ee::Vector* screen)
{
	float w = static_cast<float>(m_img->GetClippedWidth()),
		  h = static_cast<float>(m_img->GetClippedHeight());
	memcpy(m_screen, screen, sizeof(m_screen));
	for (int i = 0; i < 4; ++i) {
		m_src[i].x = m_screen[i].x / w + 0.5f;
		m_src[i].y = m_screen[i].y / h + 0.5f;
	}
}

void QuadIcon::AfterSetImage()
{
	m_src[0].Set(0, 0);
	m_src[1].Set(0, 1);
	m_src[2].Set(1, 1);
	m_src[3].Set(1, 0);

	float hw = m_img->GetClippedWidth() * 0.5f,
		hh = m_img->GetClippedHeight() * 0.5f;
	m_screen[0].Set(-hw, -hh);
	m_screen[1].Set(-hw,  hh);
	m_screen[2].Set( hw,  hh);
	m_screen[3].Set( hw, -hh);	
}

}