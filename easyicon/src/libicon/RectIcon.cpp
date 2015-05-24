#include "RectIcon.h"

namespace eicon
{

RectIcon::RectIcon()
	: m_min(0, 0)
	, m_max(1, 1)
{
}

void RectIcon::Draw(const d2d::Matrix& mt) const
{
	Icon::Draw(mt, d2d::Rect(m_min, m_max));
}

void RectIcon::LoadFromFile(const Json::Value& value)
{

}

void RectIcon::StoreToFile(Json::Value& value) const
{
	value["xmin"] = m_min.x;
	value["ymin"] = m_min.y;
	value["xmax"] = m_max.x;
	value["ymax"] = m_max.y;
}

void RectIcon::SetHoriRegion(float xmin, float xmax)
{
	m_min.x = xmin;
	m_max.x = xmax;

	if (m_img) {
		float xlen = m_img->originWidth();
		m_region.xMin = xlen * (m_min.x - 0.5f);
		m_region.xMax = xlen * (m_max.x - 0.5f);
	}
}

void RectIcon::SetVertRegion(float ymin, float ymax)
{
	m_min.y = ymin;
	m_max.y = ymax;

	if (m_img) {
		float ylen = m_img->originHeight();
		m_region.yMin = ylen * (m_min.y - 0.5f);
		m_region.yMax = ylen * (m_max.y - 0.5f);	
	}
}

void RectIcon::OnImageChanged()
{
	if (!m_img) {
		return;
	}

	float xlen = m_img->originWidth();
	m_region.xMin = xlen * (m_min.x - 0.5f);
	m_region.xMax = xlen * (m_max.x - 0.5f);

	float ylen = m_img->originHeight();
	m_region.yMin = ylen * (m_min.y - 0.5f);
	m_region.yMax = ylen * (m_max.y - 0.5f);	
}

}