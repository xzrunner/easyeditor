#include "RectShape.h"
#include "RectPropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

RectShape::RectShape()
	: m_core(sm::rect(sm::vec2(0, 0), 1, 1))
{
}

RectShape::RectShape(const sm::rect& r)
	: m_core(r)
{
}

RectShape::RectShape(const sm::vec2& center, float hWidth, float hHeight)
: m_core(sm::rect(center, hWidth * 2, hHeight * 2))
{
}

RectShape* RectShape::Clone() const
{
	return new RectShape(*this);
}

void RectShape::Translate(const sm::vec2& offset)
{
	sm::rect r = m_core.GetRect();
	r.Translate(offset);
	m_core.SetRect(r);
}

ee::PropertySetting* RectShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new RectPropertySetting(stage, this);
}

void RectShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	sm::rect r;
	r.xmin = value["xmin"].asDouble();
	r.xmax = value["xmax"].asDouble();
	r.ymin = value["ymin"].asDouble();
	r.ymax = value["ymax"].asDouble();
	m_core.SetRect(r);
}

void RectShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	const sm::rect& r = m_core.GetRect();
	value["xmin"] = r.xmin;
	value["xmax"] = r.xmax;
	value["ymin"] = r.ymin;
	value["ymax"] = r.ymax;
}

}