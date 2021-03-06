#include "SpriteIO.h"
#include "Sprite.h"
#include "Config.h"
#include "SettingData.h"

#include <sprite2/Sprite.h>

namespace ee
{

SpriteIO::SpriteIO()
	: s2loader::SpriteIO(Config::Instance()->GetSettings().spr_output_compress, Config::Instance()->IsRenderOpen())
{
	m_perspective	= sm::vec2(0, 0);

	m_clip			= false;
	m_anchor		= false;
}

void SpriteIO::LoadGeometry(const s2::SprPtr& spr)
{
	s2loader::SpriteIO::LoadGeometry(spr);

	auto ee_spr = std::dynamic_pointer_cast<Sprite>(spr);
	ee_spr->SetPerspective(m_perspective);
}

void SpriteIO::StoreGeometry(const s2::SprConstPtr& spr)
{
	s2loader::SpriteIO::StoreGeometry(spr);

	auto ee_spr = std::dynamic_pointer_cast<const Sprite>(spr);
	m_perspective = ee_spr->GetPerspective();
}

void SpriteIO::LoadGeometry(const Json::Value& val)
{
	s2loader::SpriteIO::LoadGeometry(val);

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
	s2loader::SpriteIO::StoreGeometry(val);

	if (!m_compress || m_perspective != sm::vec2(0, 0)) {
		val["x perspective"] = m_perspective.x;
		val["y perspective"] = m_perspective.y;
	}
}


void SpriteIO::LoadInfo(const s2::SprPtr& spr)
{
	s2loader::SpriteIO::LoadInfo(spr);

	auto ee_spr = std::dynamic_pointer_cast<Sprite>(spr);
	ee_spr->SetTag(m_tag);
	ee_spr->SetAnchor(m_anchor);
}

void SpriteIO::StoreInfo(const s2::SprConstPtr& spr)
{
	s2loader::SpriteIO::StoreInfo(spr);

	auto ee_spr = std::dynamic_pointer_cast<const Sprite>(spr);
	m_tag    = ee_spr->GetTag();
	m_anchor = ee_spr->IsAnchor();
}

void SpriteIO::LoadInfo(const Json::Value& val)
{
	s2loader::SpriteIO::LoadInfo(val);

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
	s2loader::SpriteIO::StoreInfo(val);

	if (!m_compress || !m_tag.empty()) {
		val["tag"] = m_tag.c_str();
	}

	if (!m_compress || m_clip) {
		val["clip"] = m_clip;
	}

	if (!m_compress || m_anchor) {
		val["anchor"] = m_anchor;
	}
}

}