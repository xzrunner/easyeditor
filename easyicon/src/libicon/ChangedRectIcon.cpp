#include "ChangedRectIcon.h"

#include <gum/JsonSerializer.h>
#include <sns/IconSym.h>
#include <bs/FixedPointNum.h>

namespace eicon
{

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	gum::JsonSerializer::Load(value["begin"], m_begin);
	gum::JsonSerializer::Load(value["end"], m_end);
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	gum::JsonSerializer::Store(m_begin, value["begin"]);
	gum::JsonSerializer::Store(m_end, value["end"]);
}

void ChangedRectIcon::LoadFromFile(const sns::IconSym& sym)
{
	auto vertices = sym.GetVertices();

	size_t idx = 0;
	m_begin.xmin = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_begin.ymin = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_begin.xmax = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_begin.ymax = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_end.xmin = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_end.ymin = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_end.xmax = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
	m_end.ymax = bs::int2float(vertices[idx++], sns::IconSym::FIXED_TRANS_PRECISION);
}

}