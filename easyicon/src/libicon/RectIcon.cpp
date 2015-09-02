#include "RectIcon.h"

namespace eicon
{

RectIcon::RectIcon()
	: m_min(0, 0)
	, m_max(1, 1)
{
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

void RectIcon::GetBound(float process, d2d::Vector bound[4]) const
{
	bound[0].set(m_min.x, m_min.y);
	bound[1].set(m_min.x, m_max.y);
	bound[2].set(m_max.x, m_max.y);
	bound[3].set(m_max.x, m_min.y);
}

void RectIcon::SetHoriRegion(float xmin, float xmax)
{
	m_min.x = xmin;
	m_max.x = xmax;

// 	if (m_img) {
// 		float xlen = m_img->GetOriginWidth();
// 		m_region.xMin = xlen * (m_min.x - 0.5f);
// 		m_region.xMax = xlen * (m_max.x - 0.5f);
// 	}
}

void RectIcon::SetVertRegion(float ymin, float ymax)
{
	m_min.y = ymin;
	m_max.y = ymax;

// 	if (m_img) {
// 		float ylen = m_img->GetOriginHeight();
// 		m_region.yMin = ylen * (m_min.y - 0.5f);
// 		m_region.yMax = ylen * (m_max.y - 0.5f);	
// 	}
}

}