#include "RenderParams.h"

namespace ee
{
	
RenderParams::RenderParams() 
{}

RenderParams::RenderParams(const RenderParams& trans)
	: mt(trans.mt)
	, color(trans.color)
	, shader(trans.shader)
	, camera(trans.camera) 
{}

RenderParams::RenderParams(const Matrix& mt) 
	: mt(mt) 
{}

RenderParams::RenderParams(const Matrix& mt, const RenderColor& color) 
	: mt(mt)
	, color(color)
{}

void RenderParams::LoadFromFile(const Json::Value& val)
{
	color.LoadFromFile(val);
	shader.LoadFromFile(val);
}

void RenderParams::StoreToFile(Json::Value& val) const
{
	color.StoreToFile(val);
	shader.StoreToFile(val);
}

}