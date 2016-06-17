#include "SpriteIO.h"
#include "trans_color.h"
#include "Config.h"
#include "BlendModes.h"
#include "FilterModes.h"
#include "CameraModes.h"
#include "Sprite.h"
#include "SettingData.h"

#include <sprite2/RenderColor.h>
#include <sprite2/RenderShader.h>
#include <sprite2/RenderCamera.h>
#include <sprite2/FilterFactory.h>
#include <sprite2/RenderFilter.h>
#include <sprite2/RFEdgeDetection.h>
#include <sprite2/RFGaussianBlur.h>
#include <sprite2/RFOuterGlow.h>
#include <sprite2/S2_Sprite.h>

#include <assert.h>

namespace ee
{

SpriteIO::Data::Data()
{
	position	= sm::vec2(0, 0);
	angle		= 0;
	scale		= sm::vec2(1, 1);
	shear		= sm::vec2(0, 0);
	offset		= sm::vec2(0, 0);
	mirror		= sm::bvec2(false, false);
	perspective	= sm::vec2(0, 0);

	blend		= s2::BM_NULL;
	filter		= NULL;
	camera		= s2::CM_ORTHO;

	clip		= false;
	anchor		= false;

	visible		= true;
	editable	= true;
}

void SpriteIO::Load(const Json::Value& val, Sprite* spr)
{
	Data data;
	Load(val, data);

	LoadGeometry(data, spr);
	LoadRender(data, spr);
	LoadInfo(data, spr);
	LoadEdit(data, spr);
}

void SpriteIO::Store(Json::Value& val, const Sprite* spr)
{
	Data data;

	StoreGeometry(data, spr);
	StoreRender(data, spr);
	StoreInfo(data, spr);
	StoreEdit(data, spr);

	Store(val, data);
}

void SpriteIO::Load(const Json::Value& val, Data& data)
{
	LoadGeometry(val, data);
	LoadRender(val, data);
	LoadInfo(val, data);
	LoadEdit(val, data);
}

void SpriteIO::Store(Json::Value& val, const Data& data)
{
	StoreGeometry(val, data);
	StoreRender(val, data);
	StoreInfo(val, data);
	StoreEdit(val, data);
}

void SpriteIO::LoadGeometry(const Data& data, Sprite* spr)
{
	spr->SetScale(data.scale);
	spr->SetShear(data.shear);
	spr->SetMirror(data.mirror.x, data.mirror.y);
	spr->SetPerspective(data.perspective);
	spr->SetOffset(data.offset);
	spr->SetTransform(data.position, data.angle);
}

void SpriteIO::StoreGeometry(Data& data, const Sprite* spr)
{
	data.position = spr->GetPosition();
	data.angle = spr->GetAngle();
	data.scale = spr->GetScale();
	data.shear = spr->GetShear();
	data.offset = spr->GetOffset();
	data.mirror = spr->GetMirror();
	data.perspective = spr->GetPerspective();
}

void SpriteIO::LoadGeometry(const Json::Value& val, Data& data)
{
	// scale
	sm::vec2 scale(1, 1);
	if (!val["x scale"].isNull() && !val["y scale"].isNull())
	{
		scale.x = static_cast<float>(val["x scale"].asDouble());
		scale.y = static_cast<float>(val["y scale"].asDouble());
	}
	else if (!val["scale"].isNull())
	{
		scale.x = scale.y = static_cast<float>(val["scale"].asDouble());
	}
	data.scale = scale;

	// shear
	sm::vec2 shear(0, 0);
	if (!val["x shear"].isNull() && !val["y shear"].isNull())
	{
		shear.x = static_cast<float>(val["x shear"].asDouble());
		shear.y = static_cast<float>(val["y shear"].asDouble());
	}
	data.shear = shear;

	// mirror
	sm::bvec2 mirror(false, false);
	if (!val["x mirror"].isNull()) {
		mirror.x = val["x mirror"].asBool();
	}
	if (!val["y mirror"].isNull()) {
		mirror.y = val["y mirror"].asBool();
	}
	data.mirror = mirror;

	// perspective
	sm::vec2 perspective(0, 0);
	if (!val["x perspective"].isNull() && !val["y perspective"].isNull())
	{
		perspective.x = static_cast<float>(val["x perspective"].asDouble());
		perspective.y = static_cast<float>(val["y perspective"].asDouble());
	}
	data.perspective = perspective;

	// offset
	sm::vec2 offset(0 ,0);
	if (!val["x offset"].isNull() && !val["y offset"].isNull())
	{
		offset.x = static_cast<float>(val["x offset"].asDouble());
		offset.y = static_cast<float>(val["y offset"].asDouble());
	}
	data.offset = offset;

	// translate
	sm::vec2 pos(0, 0);
	if (!val["position"]["x"].isNull() && !val["position"]["y"].isNull()) {
		pos.x = static_cast<float>(val["position"]["x"].asDouble());
		pos.y = static_cast<float>(val["position"]["y"].asDouble());
	}
	data.position = pos;

	// rotate
	float angle = 0;
	if (!val["angle"].isNull()) {
		angle = static_cast<float>(val["angle"].asDouble());
	}
	data.angle = angle;
}

void SpriteIO::StoreGeometry(Json::Value& val, const Data& data)
{
	bool compress = Config::Instance()->GetSettings().spr_output_compress;

	if (!compress || data.position != sm::vec2(0, 0)) {
		val["position"]["x"] = data.position.x;
		val["position"]["y"] = data.position.y;
	}

	if (!compress || data.angle != 0) {
		val["angle"] = data.angle;
	}

	if (!compress || data.scale != sm::vec2(1, 1)) {
		val["x scale"] = data.scale.x;
		val["y scale"] = data.scale.y;
	}

	if (!compress || data.shear != sm::vec2(0, 0)) {
		val["x shear"] = data.shear.x;
		val["y shear"] = data.shear.y;
	}

	if (!compress || data.offset != sm::vec2(0, 0)) {
		val["x offset"] = data.offset.x;
		val["y offset"] = data.offset.y;
	}

	if (!compress || data.mirror.x) {
		val["x mirror"] = data.mirror.x;
	}
	if (!compress || data.mirror.y) {
		val["y mirror"] = data.mirror.y;
	}

	if (!compress || data.perspective != sm::vec2(0, 0)) {
		val["x perspective"] = data.perspective.x;
		val["y perspective"] = data.perspective.y;
	}
}

void SpriteIO::LoadRender(const Data& data, Sprite* spr)
{
	LoadColor(data, spr->GetColor());
	LoadShader(data, spr->GetShader());
	LoadCamera(data, spr->GetCamera());	
}

void SpriteIO::StoreRender(Data& data, const Sprite* spr)
{
	StoreColor(data, spr->GetColor());
	StoreShader(data, spr->GetShader());
	StoreCamera(data, spr->GetCamera());	
}

void SpriteIO::LoadRender(const Json::Value& val, Data& data)
{
	LoadColor(val, data);
	LoadShader(val, data);
	LoadCamera(val, data);	
}

void SpriteIO::StoreRender(Json::Value& val, const Data& data)
{
	StoreColor(val, data);
	StoreShader(val, data);
	StoreCamera(val, data);	
}

void SpriteIO::LoadInfo(const Data& data, Sprite* spr)
{
	spr->SetName(data.name);
	spr->SetTag(data.tag);
	spr->SetClip(data.clip);
	spr->SetAnchor(data.anchor);
}

void SpriteIO::StoreInfo(Data& data, const Sprite* spr)
{
	data.name = spr->GetName();
	data.tag = spr->GetTag();
	data.clip = spr->IsClip();
	data.anchor = spr->IsAnchor();
}

void SpriteIO::LoadInfo(const Json::Value& val, Data& data)
{
	if (!val["name"].isNull()) {
		data.name = val["name"].asString();
	} else {
		data.name = "";
	}

	if (!val["tag"].isNull()) {
		data.tag = val["tag"].asString();
	} else {
		data.tag = "";
	}

	if (!val["clip"].isNull()) {
		data.clip = val["clip"].asBool();
	} else {
		data.clip = false;
	}

	if (!val["anchor"].isNull()) {
		data.anchor = val["anchor"].asBool();
	} else {
		data.anchor = false;
	}
}

void SpriteIO::StoreInfo(Json::Value& val, const Data& data)
{
	bool compress = Config::Instance()->GetSettings().spr_output_compress;

	if (!compress || !data.name.empty()) {
		val["name"] = data.name;
	}

	if (!compress || !data.tag.empty()) {
		val["tag"] = data.tag;		
	}

	if (!compress || data.clip) {
		val["clip"] = data.clip;
	}

	if (!compress || data.anchor) {
		val["anchor"] = data.anchor;
	}
}

void SpriteIO::LoadEdit(const Data& data, Sprite* spr)
{
	spr->SetVisible(data.visible);
	spr->SetEditable(data.editable);
}

void SpriteIO::StoreEdit(Data& data, const Sprite* spr)
{
	data.visible = spr->IsVisible();
	data.editable = spr->IsEditable();
}

void SpriteIO::LoadEdit(const Json::Value& val, Data& data)
{
	if (!val["visible"].isNull()) {
		data.visible = val["visible"].asBool();
	} else {
		data.visible = true;
	}
	if (!val["editable"].isNull()) {
		data.editable = val["editable"].asBool();
	} else {
		data.editable = true;
	}
}

void SpriteIO::StoreEdit(Json::Value& val, const Data& data)
{
	bool compress = Config::Instance()->GetSettings().spr_output_compress;

	if (!compress || !data.visible) {
		val["visible"] = data.visible;
	}
	if (!compress || !data.editable) {
		val["editable"] = data.editable;
	}
}

void SpriteIO::LoadColor(const Data& data, s2::RenderColor& color)
{
	color = data.col;
}

void SpriteIO::StoreColor(Data& data, const s2::RenderColor& color)
{
	data.col = color;
}

void SpriteIO::LoadColor(const Json::Value& val, Data& data)
{
	if (!val["multi color"].isNull()) {
		data.col.mul = str2color(val["multi color"].asString(), PT_BGRA);
	} else {
		data.col.mul = s2::Color(0xffffffff);
	}

	if (!val["add color"].isNull()) {
		data.col.add = str2color(val["add color"].asString(), PT_ABGR);
	} else {
		data.col.add = s2::Color(0);
	}

	if (!val["r trans"].isNull()) {
		data.col.rmap = str2color(val["r trans"].asString(), PT_RGBA);
	} else {
		data.col.rmap = s2::Color(255, 0, 0, 0);
	}

	if (!val["g trans"].isNull()) {
		data.col.gmap = str2color(val["g trans"].asString(), PT_RGBA);
	} else {
		data.col.gmap = s2::Color(0, 255, 0, 0);
	}

	if (!val["b trans"].isNull()) {
		data.col.bmap = str2color(val["b trans"].asString(), PT_RGBA);
	} else {
		data.col.bmap = s2::Color(0, 0, 255, 0);
	}
}

void SpriteIO::StoreColor(Json::Value& val, const Data& data)
{
	bool compress = Config::Instance()->GetSettings().spr_output_compress;

	if (!compress || data.col.mul != s2::Color(0xffffffff)) {
		val["multi color"]	= color2str(data.col.mul, PT_BGRA);
	}
	if (!compress || data.col.add != s2::Color(0)) {
		val["add color"]	= color2str(data.col.add, PT_ABGR);
	}

	if (!compress || data.col.rmap.r != 255 || data.col.rmap.g != 0 || data.col.rmap.b != 0) {
		val["r trans"]		= color2str(data.col.rmap, PT_RGBA);
	}
	if (!compress || data.col.gmap.r != 0 || data.col.gmap.g != 255 || data.col.gmap.b != 0) {
		val["g trans"]		= color2str(data.col.gmap, PT_RGBA);
	}
	if (!compress || data.col.bmap.r != 0 || data.col.bmap.g != 0 || data.col.bmap.b != 255) {
		val["b trans"]		= color2str(data.col.bmap, PT_RGBA);
	}
}

void SpriteIO::LoadShader(const Data& data, s2::RenderShader& shader)
{
	shader.blend = data.blend;
	shader.filter = data.filter;
}

void SpriteIO::StoreShader(Data& data, const s2::RenderShader& shader)
{
	data.blend = shader.blend;
	data.filter = shader.filter;
}

void SpriteIO::LoadShader(const Json::Value& val, Data& data)
{
	data.blend = s2::BM_NULL;
	if (data.filter) {
		delete data.filter;
		data.filter = NULL;
	}

	if (!Config::Instance()->IsRenderOpen()) {
		return;
	}

	if (!val["blend"].isNull()) {
		std::string disc = val["blend"].asString();
		data.blend = BlendModes::Instance()->GetModeFromNameEN(disc);
	}

	if (!val["filter"].isNull()) 
	{
		if (val["filter"].isString()) 
		{
			std::string disc = val["filter"].asString();
			s2::FilterMode filter = FilterModes::Instance()->GetModeFromNameEN(disc);
			data.filter = s2::FilterFactory::Instance()->Create(filter);
		} 
		else 
		{
			const Json::Value& fval = val["filter"];
			std::string disc = fval["mode"].asString();
			s2::FilterMode filter = FilterModes::Instance()->GetModeFromNameEN(disc);
			data.filter = s2::FilterFactory::Instance()->Create(filter);
			switch (filter)
			{
			case s2::FM_EDGE_DETECTION:
				{
					float blend = (float)(fval["blend"].asDouble());
					s2::RFEdgeDetection* filter = static_cast<s2::RFEdgeDetection*>(data.filter);
					filter->SetBlend(blend);
				}
				break;
			case s2::FM_GAUSSIAN_BLUR:
				{
					int iterations = fval["iterations"].asInt();
					s2::RFGaussianBlur* filter = static_cast<s2::RFGaussianBlur*>(data.filter);
					filter->SetIterations(iterations);
				}
				break;
			case s2::FM_OUTER_GLOW:
				{
					int iterations = fval["iterations"].asInt();
					s2::RFOuterGlow* filter = static_cast<s2::RFOuterGlow*>(data.filter);
					filter->SetIterations(iterations);
				}
				break;
			}
		}
	}
	else
	{
		data.filter = s2::FilterFactory::Instance()->Create(s2::FM_NULL);
	}
}

void SpriteIO::StoreShader(Json::Value& val, const Data& data)
{
	if (!Config::Instance()->IsRenderOpen()) {
		return;
	}

	if (data.blend != s2::BM_NULL) {
		val["blend"] = BlendModes::Instance()->GetNameENFromMode(data.blend);
	}

	s2::FilterMode mode = s2::FM_NULL;
	if (data.filter) {
		mode = data.filter->GetMode();
	}
	if (mode != s2::FM_NULL)
	{
		Json::Value fval;
		fval["mode"] = FilterModes::Instance()->GetNameENFromMode(mode);
		switch (mode)
		{
		case s2::FM_EDGE_DETECTION:
			{
				s2::RFEdgeDetection* filter = static_cast<s2::RFEdgeDetection*>(data.filter);
				fval["blend"] = filter->GetBlend();
			}
			break;
		case s2::FM_GAUSSIAN_BLUR:
			{
				s2::RFGaussianBlur* filter = static_cast<s2::RFGaussianBlur*>(data.filter);
				fval["iterations"] = filter->GetIterations();
			}
			break;
		case s2::FM_OUTER_GLOW:
			{
				s2::RFOuterGlow* filter = static_cast<s2::RFOuterGlow*>(data.filter);
				fval["iterations"] = filter->GetIterations();
			}
			break;
		}
		val["filter"] = fval;
	}
}

void SpriteIO::LoadCamera(const Data& data, s2::RenderCamera& camera)
{
	camera.mode = data.camera;
}

void SpriteIO::StoreCamera(Data& data, const s2::RenderCamera& camera)
{
	data.camera = camera.mode;
}

void SpriteIO::LoadCamera(const Json::Value& val, Data& data)
{
	if (!val["camera"].isNull()) {
		std::string disc = val["camera"].asString();
		data.camera = CameraModes::Instance()->GetModeFromNameEN(disc);
	} else {
		data.camera = s2::CM_ORTHO;
	}
}

void SpriteIO::StoreCamera(Json::Value& val, const Data& data)
{
	if (data.camera != s2::CM_ORTHO) {
		val["camera"] = CameraModes::Instance()->GetNameENFromMode(data.camera);
	}
}

}