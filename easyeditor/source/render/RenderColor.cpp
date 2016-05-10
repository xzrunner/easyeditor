#include "RenderColor.h"
#include "trans_color.h"

namespace ee
{

void RenderColor::LoadFromFile(const Json::Value& val)
{
	std::string str = val["multi color"].asString();
	if (str.empty()) {
		mul = s2::Color(0xffffffff);
	} else {
		mul = str2color(str, PT_BGRA);
	}
	str = val["add color"].asString();
	if (str.empty()) {
		add = s2::Color(0);
	} else {
		add = str2color(str, PT_ABGR);
	}

	str = val["r trans"].asString();
	if (str.empty()) {
		rmap = s2::Color(255, 0, 0, 0);
	} else {
		rmap = str2color(str, PT_RGBA);
	}
	str = val["g trans"].asString();
	if (str.empty()) {
		gmap = s2::Color(0, 255, 0, 0);
	} else {
		gmap = str2color(str, PT_RGBA);
	}
	str = val["b trans"].asString();
	if (str.empty()) {
		bmap = s2::Color(0, 0, 255, 0);
	} else {
		bmap = str2color(str, PT_RGBA);
	}
}

void RenderColor::StoreToFile(Json::Value& val) const
{
	val["multi color"]	= color2str(mul, PT_BGRA);
	val["add color"]	= color2str(add, PT_ABGR);
	val["r trans"]		= color2str(rmap, PT_RGBA);
	val["g trans"]		= color2str(gmap, PT_RGBA);
	val["b trans"]		= color2str(bmap, PT_RGBA);
}

}