#include "ColorTrans.h"
#include "trans_color.h"

namespace ee
{

ColorTrans::ColorTrans()
	: multi(1, 1, 1, 1)
	, add(0, 0, 0, 0)
	, r(1, 0, 0, 0)
	, g(0, 1, 0, 0)
	, b(0, 0, 1, 0)
{
}

void ColorTrans::LoadFromFile(const Json::Value& val)
{
	std::string str = val["multi color"].asString();
	if (str.empty()) {
		multi = Colorf(1, 1, 1, 1);
	} else {
		multi = TransColor(str, PT_BGRA);
	}
	str = val["add color"].asString();
	if (str.empty()) {
		add = Colorf(0, 0, 0, 0);
	} else {
		add = TransColor(str, PT_ABGR);
	}

	str = val["r trans"].asString();
	if (str.empty()) {
		r = Colorf(1, 0, 0, 1);
	} else {
		r = TransColor(str, PT_RGBA);
	}
	str = val["g trans"].asString();
	if (str.empty()) {
		g = Colorf(0, 1, 0, 1);
	} else {
		g = TransColor(str, PT_RGBA);
	}
	str = val["b trans"].asString();
	if (str.empty()) {
		b = Colorf(0, 0, 1, 1);
	} else {
		b = TransColor(str, PT_RGBA);
	}
}

void ColorTrans::StoreToFile(Json::Value& val) const
{
	val["multi color"] = TransColor(multi, PT_BGRA);
	val["add color"] = TransColor(add, PT_ABGR);
	val["r trans"] = TransColor(r, PT_RGBA);
	val["g trans"] = TransColor(g, PT_RGBA);
	val["b trans"] = TransColor(b, PT_RGBA);
}

}