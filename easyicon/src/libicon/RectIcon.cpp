#include "RectIcon.h"

namespace eicon
{

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

}