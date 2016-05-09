#include "RectIcon.h"

#include <ee/Image.h>

namespace eicon
{

RectIcon::RectIcon()
	: m_min(0, 0)
	, m_max(1, 1)
{
}

void RectIcon::LoadFromFile(const Json::Value& value)
{
	m_min.x = static_cast<float>(value["xmin"].asDouble());
	m_min.y = static_cast<float>(value["ymin"].asDouble());
	m_max.x = static_cast<float>(value["xmax"].asDouble());
	m_max.y = static_cast<float>(value["ymax"].asDouble());
}

void RectIcon::StoreToFile(Json::Value& value) const
{
	value["xmin"] = m_min.x;
	value["ymin"] = m_min.y;
	value["xmax"] = m_max.x;
	value["ymax"] = m_max.y;
}

void RectIcon::GetBound(float process, sm::vec2 bound[4]) const
{
	bound[0].Set(m_min.x, m_min.y);
	bound[1].Set(m_min.x, m_max.y);
	bound[2].Set(m_max.x, m_max.y);
	bound[3].Set(m_max.x, m_min.y);
}

void RectIcon::SetRegion(const ee::Rect& r)
{
	float w = static_cast<float>(m_img->GetClippedWidth()),
		  h = static_cast<float>(m_img->GetClippedHeight());
	m_min.x = r.xmin / w + 0.5f;
	m_min.y = r.ymin / h + 0.5f;
	m_max.x = r.xmax / w + 0.5f;
	m_max.y = r.ymax / h + 0.5f;

	m_min.x = std::min(1.0f, std::max(0.0f, m_min.x));
	m_min.y = std::min(1.0f, std::max(0.0f, m_min.y));
	m_max.x = std::max(0.0f, std::min(1.0f, m_max.x));
	m_max.y = std::max(0.0f, std::min(1.0f, m_max.y));
}

}