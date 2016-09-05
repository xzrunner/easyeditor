#include "SpriteIO.h"
#include "Sprite.h"
#include "Config.h"
#include "SettingData.h"

#include <sprite2/S2_Sprite.h>

namespace ee
{

SpriteIO::SpriteIO()
	: glue::SpriteIO(Config::Instance()->GetSettings().spr_output_compress, Config::Instance()->IsRenderOpen())
{
	m_mirror		= sm::bvec2(false, false);
	m_perspective	= sm::vec2(0, 0);

	m_clip			= false;
	m_anchor		= false;

	m_editable		= true;
}

void SpriteIO::LoadGeometry(s2::Sprite* spr)
{
	glue::SpriteIO::LoadGeometry(spr);

	Sprite* ee_spr = dynamic_cast<Sprite*>(spr);
	ee_spr->SetMirror(m_mirror);
	ee_spr->SetPerspective(m_perspective);
}

void SpriteIO::StoreGeometry(const s2::Sprite* spr)
{
	glue::SpriteIO::StoreGeometry(spr);

	const Sprite* ee_spr = dynamic_cast<const Sprite*>(spr);
	m_mirror = ee_spr->GetMirror();
	m_perspective = ee_spr->GetPerspective();
}

void SpriteIO::LoadGeometry(const Json::Value& val)
{
	glue::SpriteIO::LoadGeometry(val);

	// mirror
	sm::bvec2 mirror(false, false);
	if (!val["x mirror"].isNull()) {
		mirror.x = val["x mirror"].asBool();
	}
	if (!val["y mirror"].isNull()) {
		mirror.y = val["y mirror"].asBool();
	}
	m_mirror = mirror;

	// perspective
	sm::vec2 perspective(0, 0);
	if (!val["x perspective"].isNull() && !val["y perspective"].isNull())
	{
		perspective.x = static_cast<float>(val["x perspective"].asDouble());
		perspective.y = static_cast<float>(val["y perspective"].asDouble());
	}
	m_perspective = perspective;
}

void SpriteIO::StoreGeometry(Json::Value& val)
{
	glue::SpriteIO::StoreGeometry(val);

	if (m_mirror.x) {
		m_scale.x = -m_scale.x;
	}
	if (m_mirror.y) {
		m_scale.y = -m_scale.y;
	}
	if (!m_compress || m_scale != sm::vec2(1, 1)) {
		val["x scale"] = m_scale.x;
		val["y scale"] = m_scale.y;
	}

	if (!m_compress || m_mirror.x) {
		val["x mirror"] = m_mirror.x;
	}
	if (!m_compress || m_mirror.y) {
		val["y mirror"] = m_mirror.y;
	}

	if (!m_compress || m_perspective != sm::vec2(0, 0)) {
		val["x perspective"] = m_perspective.x;
		val["y perspective"] = m_perspective.y;
	}
}


void SpriteIO::LoadInfo(s2::Sprite* spr)
{
	glue::SpriteIO::LoadInfo(spr);

	Sprite* ee_spr = dynamic_cast<Sprite*>(spr);
	ee_spr->SetTag(m_tag);
	ee_spr->SetClip(m_clip);
	ee_spr->SetAnchor(m_anchor);
}

void SpriteIO::StoreInfo(const s2::Sprite* spr)
{
	glue::SpriteIO::StoreInfo(spr);

	const Sprite* ee_spr = dynamic_cast<const Sprite*>(spr);
	m_tag = ee_spr->GetTag();
	m_clip = ee_spr->IsClip();
	m_anchor = ee_spr->IsAnchor();
}

void SpriteIO::LoadInfo(const Json::Value& val)
{
	glue::SpriteIO::LoadInfo(val);

	if (!val["tag"].isNull()) {
		m_tag = val["tag"].asString();
	} else {
		m_tag = "";
	}

	if (!val["clip"].isNull()) {
		m_clip = val["clip"].asBool();
	} else {
		m_clip = false;
	}

	if (!val["anchor"].isNull()) {
		m_anchor = val["anchor"].asBool();
	} else {
		m_anchor = false;
	}
}

void SpriteIO::StoreInfo(Json::Value& val)
{
	glue::SpriteIO::StoreInfo(val);

	if (!m_compress || !m_tag.empty()) {
		val["tag"] = m_tag;		
	}

	if (!m_compress || m_clip) {
		val["clip"] = m_clip;
	}

	if (!m_compress || m_anchor) {
		val["anchor"] = m_anchor;
	}
}

void SpriteIO::LoadEdit(s2::Sprite* spr)
{
	glue::SpriteIO::LoadEdit(spr);

	Sprite* ee_spr = dynamic_cast<Sprite*>(spr);
	ee_spr->SetEditable(m_editable);
}

void SpriteIO::StoreEdit(const s2::Sprite* spr)
{
	glue::SpriteIO::StoreEdit(spr);

	const Sprite* ee_spr = dynamic_cast<const Sprite*>(spr);
	m_editable = ee_spr->IsEditable();
}

void SpriteIO::LoadEdit(const Json::Value& val)
{
	glue::SpriteIO::LoadEdit(val);

	if (!val["editable"].isNull()) {
		m_editable = val["editable"].asBool();
	} else {
		m_editable = true;
	}
}

void SpriteIO::StoreEdit(Json::Value& val)
{
	glue::SpriteIO::StoreEdit(val);

	if (!m_compress || !m_editable) {
		val["editable"] = m_editable;
	}
}

}