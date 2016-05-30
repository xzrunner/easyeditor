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
#include <sprite2/RFEdgeDetection.h>
#include <sprite2/RFGaussianBlur.h>

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
	if (!Config::Instance()->IsRenderOpen()) {
		return;
	}

	if (!val["blend"].isNull()) {
		std::string disc = val["blend"].asString();
		shader.blend = BlendModes::Instance()->GetModeFromNameEN(disc);
	}

	if (!val["filter"].isNull()) 
	{
		if (shader.filter) {
			delete shader.filter;
		}

		if (val["filter"].isString()) 
		{
			std::string disc = val["filter"].asString();
			s2::FilterMode filter = FilterModes::Instance()->GetModeFromNameEN(disc);
			shader.filter = s2::FilterFactory::Instance()->Create(filter);
		} 
		else 
		{
			const Json::Value& fval = val["filter"];
			std::string disc = fval["mode"].asString();
			s2::FilterMode filter = FilterModes::Instance()->GetModeFromNameEN(disc);
			shader.filter = s2::FilterFactory::Instance()->Create(filter);
			switch (filter)
			{
			case s2::FM_EDGE_DETECTION:
				{
					float blend = (float)(fval["blend"].asDouble());
					s2::RFEdgeDetection* filter = static_cast<s2::RFEdgeDetection*>(shader.filter);
					filter->SetBlend(blend);
				}
				break;
			case s2::FM_GAUSSIAN_BLUR:
				{
					int iterations = fval["iterations"].asInt();
					s2::RFGaussianBlur* filter = static_cast<s2::RFGaussianBlur*>(shader.filter);
					filter->SetIterations(iterations);
				}
				break;
			}
		}
	}
}

void SpriteIO::StoreShader(Json::Value& val, const s2::RenderShader& shader)
{
	if (!Config::Instance()->IsRenderOpen()) {
		return;
	}

	val["blend"] = BlendModes::Instance()->GetNameENFromMode(shader.blend);

	Json::Value fval;
	s2::FilterMode mode = shader.filter->GetMode();
	fval["mode"] = FilterModes::Instance()->GetNameENFromMode(mode);
	switch (mode)
	{
	case s2::FM_EDGE_DETECTION:
		{
			s2::RFEdgeDetection* filter = static_cast<s2::RFEdgeDetection*>(shader.filter);
			fval["blend"] = filter->GetBlend();
		}
		break;
	case s2::FM_GAUSSIAN_BLUR:
		{
			s2::RFGaussianBlur* filter = static_cast<s2::RFGaussianBlur*>(shader.filter);
			fval["iterations"] = filter->GetIterations();
		}
		break;
	}
	val["filter"] = fval;
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