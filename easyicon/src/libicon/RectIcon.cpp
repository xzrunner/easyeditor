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
	m_min.x = value["xmin"].asDouble();
	m_min.y = value["ymin"].asDouble();
	m_max.x = value["xmax"].asDouble();
	m_max.y = value["ymax"].asDouble();
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

void RectIcon::SetRegion(const d2d::Rect& r)
{
	float w = m_img->GetClippedWidth(),
		h = m_img->GetClippedHeight();
	m_min.x = r.xMin / w + 0.5f;
	m_min.y = r.yMin / h + 0.5f;
	m_max.x = r.xMax / w + 0.5f;
	m_max.y = r.yMax / h + 0.5f;

	m_min.x = std::min(1.0f, std::max(0.0f, m_min.x));
	m_min.y = std::min(1.0f, std::max(0.0f, m_min.y));
	m_max.x = std::max(0.0f, std::min(1.0f, m_max.x));
	m_max.y = std::max(0.0f, std::min(1.0f, m_max.y));
}

}