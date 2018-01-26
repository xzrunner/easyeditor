#include "RectIcon.h"

#include <gum/JsonSerializer.h>
#include <sns/IconSym.h>
#include <bs/FixedPointNum.h>

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

void RectIcon::LoadFromFile(const sns::IconSym& sym)
{
	auto vertices = sym.GetVertices();

	size_t idx = 0;
	m_min.x = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_min.y = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_max.x = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_max.y = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
}

}