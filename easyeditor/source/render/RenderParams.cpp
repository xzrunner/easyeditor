#include "RenderParams.h"
#include "CameraModes.h"

namespace ee
{
	
RenderParams::RenderParams() 
	: set_shader(true)
{}

RenderParams::RenderParams(const RenderParams& trans)
	: set_shader(trans.set_shader)
	, mt(trans.mt)
	, color(trans.color)
	, shader(trans.shader)
	, camera(trans.camera) 
{}

RenderParams::RenderParams(const sm::mat4& mt) 
	: set_shader(true)
	, mt(mt) 
{}

RenderParams::RenderParams(const sm::mat4& mt, const RenderColor& color) 
	: set_shader(true)
	, mt(mt)
	, color(color)
{}

void RenderParams::LoadFromFile(const Json::Value& val)
{
	color.LoadFromFile(val);
	shader.LoadFromFile(val);

	std::string disc = val["camera"].asString();
	camera.mode = CameraModes::Instance()->GetModeFromNameEN(disc);
}

void RenderParams::StoreToFile(Json::Value& val) const
{
	color.StoreToFile(val);
	shader.StoreToFile(val);

	val["camera"] = CameraModes::Instance()->GetNameENFromMode(camera.mode);
}

}