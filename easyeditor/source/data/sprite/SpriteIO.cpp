#include "SpriteIO.h"
#include "trans_color.h"
#include "Config.h"
#include "BlendModes.h"
#include "FilterModes.h"
#include "CameraModes.h"

#include <sprite2/RenderColor.h>
#include <sprite2/RenderShader.h>
#include <sprite2/RenderCamera.h>
#include <sprite2/FilterFactory.h>
#include <sprite2/RenderFilter.h>

#include <assert.h>

namespace ee
{

void SpriteIO::LoadColor(const Json::Value& val, s2::RenderColor& color)
{
	std::string str = val["multi color"].asString();
	if (str.empty()) {
		color.mul = s2::Color(0xffffffff);
	} else {
		color.mul = str2color(str, PT_BGRA);
	}
	str = val["add color"].asString();
	if (str.empty()) {
		color.add = s2::Color(0);
	} else {
		color.add = str2color(str, PT_ABGR);
	}

	str = val["r trans"].asString();
	if (str.empty()) {
		color.rmap = s2::Color(255, 0, 0, 0);
	} else {
		color.rmap = str2color(str, PT_RGBA);
	}
	str = val["g trans"].asString();
	if (str.empty()) {
		color.gmap = s2::Color(0, 255, 0, 0);
	} else {
		color.gmap = str2color(str, PT_RGBA);
	}
	str = val["b trans"].asString();
	if (str.empty()) {
		color.bmap = s2::Color(0, 0, 255, 0);
	} else {
		color.bmap = str2color(str, PT_RGBA);
	}
}

void SpriteIO::StoreColor(Json::Value& val, const s2::RenderColor& color)
{
	val["multi color"]	= color2str(color.mul, PT_BGRA);
	val["add color"]	= color2str(color.add, PT_ABGR);
	val["r trans"]		= color2str(color.rmap, PT_RGBA);
	val["g trans"]		= color2str(color.gmap, PT_RGBA);
	val["b trans"]		= color2str(color.bmap, PT_RGBA);
}

void SpriteIO::LoadShader(const Json::Value& val, s2::RenderShader& shader)
{
	if (!val["blend"].isNull()) {
		std::string disc = val["blend"].asString();
		if (Config::Instance()->IsRenderOpen()) {
			shader.blend = BlendModes::Instance()->GetModeFromNameEN(disc);
		}
	}

	if (!val["filter"].isNull()) {
		std::string disc = val["filter"].asString();
		if (Config::Instance()->IsRenderOpen()) {
			s2::FilterMode filter = FilterModes::Instance()->GetModeFromNameEN(disc);
			assert(!shader.filter);
			shader.filter = s2::FilterFactory::Instance()->Create(filter);
		}
	}
}

void SpriteIO::StoreShader(Json::Value& val, const s2::RenderShader& shader)
{
	if (Config::Instance()->IsRenderOpen()) {
		val["blend"] = BlendModes::Instance()->GetNameENFromMode(shader.blend);
		val["filter"] = FilterModes::Instance()->GetNameENFromMode(shader.filter->GetMode());
	}
}

void SpriteIO::LoadCamera(const Json::Value& val, s2::RenderCamera& camera)
{
	std::string disc = val["camera"].asString();
	camera.mode = CameraModes::Instance()->GetModeFromNameEN(disc);
}

void SpriteIO::StoreCamera(Json::Value& val, const s2::RenderCamera& camera)
{
	val["camera"] = CameraModes::Instance()->GetNameENFromMode(camera.mode);
}

}