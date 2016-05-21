#include "RectShape.h"
#include "RectPropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace eshape
{

RectShape::RectShape()
{
}

RectShape::RectShape(const sm::rect& r)
	: s2::RectShape(r)
{
}

RectShape::RectShape(const sm::vec2& center, float hwidth, float hheight)
	: s2::RectShape(sm::rect(center, hwidth * 2, hheight * 2))
{
}

RectShape* RectShape::Clone() const
{
	return new RectShape(*this);
}

void RectShape::Translate(const sm::vec2& offset)
{
	sm::rect r = GetRect();
	r.Translate(offset);
	SetRect(r);
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
	SetRect(r);
}

void RectShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	const sm::rect& r = GetRect();
	value["xmin"] = r.xmin;
	value["xmax"] = r.xmax;
	value["ymin"] = r.ymin;
	value["ymax"] = r.ymax;
}

}