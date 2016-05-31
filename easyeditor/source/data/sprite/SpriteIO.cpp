#include "SpriteIO.h"
#include "trans_color.h"
#include "Config.h"
#include "BlendModes.h"
#include "FilterModes.h"
#include "CameraModes.h"
#include "Sprite.h"

#include <sprite2/RenderColor.h>
#include <sprite2/RenderShader.h>
#include <sprite2/RenderCamera.h>
#include <sprite2/FilterFactory.h>
#include <sprite2/RenderFilter.h>
#include <sprite2/RFEdgeDetection.h>
#include <sprite2/RFGaussianBlur.h>
#include <sprite2/S2_Sprite.h>

#include <assert.h>

namespace ee
{

void SpriteIO::Load(const Json::Value& val, Sprite* spr)
{
	LoadGeometry(val, spr);
	LoadShader(val, spr);
	LoadInfo(val, spr);
	LoadEdit(val, spr);
}

void SpriteIO::Store(Json::Value& val, const Sprite* spr)
{
	StoreGeometry(val, spr);
	StoreShader(val, spr);
	StoreInfo(val, spr);
	StoreEdit(val, spr);
}

void SpriteIO::LoadGeometry(const Json::Value& val, Sprite* spr)
{
	// scale
	sm::vec2 scale(1, 1);
	if (!val["x scale"].isNull() && !val["y scale"])
	{
		scale.x = static_cast<float>(val["x scale"].asDouble());
		scale.y = static_cast<float>(val["y scale"].asDouble());
	}
	else if (!val["scale"].isNull())
	{
		scale.x = scale.y = static_cast<float>(val["scale"].asDouble());
	}
	spr->SetScale(scale);

	// shear
	sm::vec2 shear(0, 0);
	if (!val["x shear"].isNull() && !val["y shear"].isNull())
	{
		shear.x = static_cast<float>(val["x shear"].asDouble());
		shear.y = static_cast<float>(val["y shear"].asDouble());
	}
	spr->SetShear(shear);

	// mirror
	bool mx = false, my = false;
	if (!val["x mirror"].isNull()) {
		mx = val["x mirror"].asBool();
	}
	if (!val["y mirror"].isNull()) {
		my = val["y mirror"].asBool();
	}
	spr->SetMirror(mx, my);

	// perspective
	if (!val["x perspective"].isNull() && !val["y perspective"].isNull())
	{
		sm::vec2 persp;
		persp.x = static_cast<float>(val["x perspective"].asDouble());
		persp.y = static_cast<float>(val["y perspective"].asDouble());
		spr->SetPerspective(persp);
	}

	// offset
	sm::vec2 offset(0 ,0);
	if (!val["x offset"].isNull() && !val["y offset"].isNull())
	{
		offset.x = static_cast<float>(val["x offset"].asDouble());
		offset.y = static_cast<float>(val["y offset"].asDouble());
	}
	spr->SetOffset(offset);

	// translate
	sm::vec2 pos(0, 0);
	if (!val["position"]["x"].isNull() && !val["position"]["y"].isNull()) {
		pos.x = static_cast<float>(val["position"]["x"].asDouble());
		pos.y = static_cast<float>(val["position"]["y"].asDouble());
	}
	// rotate
	float angle = 0;
	if (!val["angle"].isNull()) {
		angle = static_cast<float>(val["angle"].asDouble());
	}
	spr->SetTransform(pos, angle);
}

void SpriteIO::StoreGeometry(Json::Value& val, const Sprite* spr)
{
	const sm::vec2& pos = spr->GetPosition();
	if (pos != sm::vec2(0, 0)) {
		val["position"]["x"] = pos.x;
		val["position"]["y"] = pos.y;
	}

	float angle = spr->GetAngle();
	if (angle != 0) {
		val["angle"] = angle;
	}

	const sm::vec2& scale = spr->GetScale();
	if (scale != sm::vec2(1, 1)) {
		val["x scale"] = scale.x;
		val["y scale"] = scale.y;
	}

	const sm::vec2& shear = spr->GetShear();
	if (shear != sm::vec2(0, 0)) {
		val["x shear"] = shear.x;
		val["y shear"] = shear.y;
	}

	const sm::vec2& offset = spr->GetOffset();
	if (offset != sm::vec2(0, 0)) {
		val["x offset"] = offset.x;
		val["y offset"] = offset.y;
	}

	const sm::bvec2& mirror = spr->GetMirror();
	if (mirror.x) {
		val["x mirror"] = mirror.x;
	}
	if (mirror.y) {
		val["y mirror"] = mirror.y;
	}

	const sm::vec2& perspective = spr->GetPerspective();
	if (perspective != sm::vec2(0, 0)) {
		val["x perspective"] = perspective.x;
		val["y perspective"] = perspective.y;
	}
}

void SpriteIO::LoadShader(const Json::Value& val, Sprite* spr)
{
	LoadColor(val, spr->GetCore()->Color());
	LoadShader(val, spr->GetCore()->Shader());
	LoadCamera(val, spr->GetCore()->Camera());	
}

void SpriteIO::StoreShader(Json::Value& val, const Sprite* spr)
{
	StoreColor(val, spr->GetCore()->Color());
	StoreShader(val, spr->GetCore()->Shader());
	StoreCamera(val, spr->GetCore()->Camera());	
}

void SpriteIO::LoadInfo(const Json::Value& val, Sprite* spr)
{
	if (!val["name"].isNull()) {
		spr->SetName(val["name"].asString());
	} else {
		spr->SetName("");
	}

	if (!val["tag"].isNull()) {
		spr->SetTag(val["tag"].asString());
	} else {
		spr->SetTag("");
	}

	if (!val["clip"].isNull()) {
		spr->SetClip(val["clip"].asBool());
	} else {
		spr->SetClip(false);
	}

	if (!val["anchor"].isNull()) {
		spr->SetAnchor(val["anchor"].asBool());
	} else {
		spr->SetAnchor(false);
	}
}

void SpriteIO::StoreInfo(Json::Value& val, const Sprite* spr)
{
	const std::string& name = spr->GetName();
	if (!name.empty()) {
		val["name"] = name;
	}

	const std::string& tag = spr->GetTag();
	if (!tag.empty()) {
		val["tag"] = tag;		
	}

	if (spr->IsClip()) {
		val["clip"] = true;
	}

	if (spr->IsAnchor()) {
		val["anchor"] = true;
	}
}

void SpriteIO::LoadEdit(const Json::Value& val, Sprite* spr)
{
	if (!val["visible"].isNull()) {
		spr->SetVisible(val["visible"].asBool());
	}
	if (!val["editable"].isNull()) {
		spr->SetEditable(val["editable"].asBool());
	}	
}

void SpriteIO::StoreEdit(Json::Value& val, const Sprite* spr)
{
	if (!spr->IsVisible()) {
		val["visible"] = false;
	}
	if (!spr->IsEditable()) {
		val["editable"] = false;
	}
}

void SpriteIO::LoadColor(const Json::Value& val, s2::RenderColor& color)
{
	if (!val["multi color"].isNull()) {
		color.mul = str2color(val["multi color"].asString(), PT_BGRA);
	} else {
		color.mul = s2::Color(0xffffffff);
	}

	if (!val["add color"].isNull()) {
		color.add = str2color(val["add color"].asString(), PT_ABGR);
	} else {
		color.add = s2::Color(0);
	}

	if (!val["r trans"].isNull()) {
		color.rmap = str2color(val["r trans"].asString(), PT_RGBA);
	} else {
		color.rmap = s2::Color(255, 0, 0, 0);
	}

	if (!val["g trans"].isNull()) {
		color.gmap = str2color(val["g trans"].asString(), PT_RGBA);
	} else {
		color.gmap = s2::Color(0, 255, 0, 0);
	}

	if (!val["b trans"].isNull()) {
		color.bmap = str2color(val["b trans"].asString(), PT_RGBA);
	} else {
		color.bmap = s2::Color(0, 0, 255, 0);
	}
}

void SpriteIO::StoreColor(Json::Value& val, const s2::RenderColor& color)
{
	if (color.mul != s2::Color(0xffffffff)) {
		val["multi color"]	= color2str(color.mul, PT_BGRA);
	}
	if (color.add != s2::Color(0)) {
		val["add color"]	= color2str(color.add, PT_ABGR);
	}

	if (color.rmap != s2::Color(255, 0, 0, 0)) {
		val["r trans"]		= color2str(color.rmap, PT_RGBA);
	}
	if (color.gmap != s2::Color(0, 255, 0, 0)) {
		val["g trans"]		= color2str(color.gmap, PT_RGBA);
	}
	if (color.bmap != s2::Color(0, 0, 255, 0)) {
		val["b trans"]		= color2str(color.bmap, PT_RGBA);
	}
}

void SpriteIO::LoadShader(const Json::Value& val, s2::RenderShader& shader)
{
	if (!Config::Instance()->IsRenderOpen()) {
		return;
	}

	if (!val["blend"].isNull()) {
		std::string disc = val["blend"].asString();
		shader.blend = BlendModes::Instance()->GetModeFromNameEN(disc);
	} else {
		shader.blend = s2::BM_NULL;
	}

	if (shader.filter) {
		delete shader.filter;
		shader.filter = NULL;
	}
	if (!val["filter"].isNull()) 
	{
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
	else
	{
		shader.filter = s2::FilterFactory::Instance()->Create(s2::FM_NULL);
	}
}

void SpriteIO::StoreShader(Json::Value& val, const s2::RenderShader& shader)
{
	if (!Config::Instance()->IsRenderOpen()) {
		return;
	}

	if (shader.blend != s2::BM_NULL) {
		val["blend"] = BlendModes::Instance()->GetNameENFromMode(shader.blend);
	}

	s2::FilterMode mode = shader.filter->GetMode();
	if (mode != s2::FM_NULL)
	{
		Json::Value fval;
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
}

void SpriteIO::LoadCamera(const Json::Value& val, s2::RenderCamera& camera)
{
	if (!val["camera"].isNull()) {
		std::string disc = val["camera"].asString();
		camera.mode = CameraModes::Instance()->GetModeFromNameEN(disc);
	} else {
		camera.mode = s2::CM_ORTHO;
	}
}

void SpriteIO::StoreCamera(Json::Value& val, const s2::RenderCamera& camera)
{
	if (camera.mode != s2::CM_ORTHO) {
		val["camera"] = CameraModes::Instance()->GetNameENFromMode(camera.mode);
	}
}

}