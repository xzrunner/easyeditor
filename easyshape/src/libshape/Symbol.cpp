#include "Symbol.h"
#include "FileIO.h"
#include "PolygonShape.h"

#include <easyimage.h>

#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SettingData.h>
#include <ee/Visitor.h>
#include <ee/Sprite.h>
#include <ee/SymbolType.h>

#include <sprite2/RenderParams.h>
#include <gum/JsonSerializer.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace eshape
{

Symbol::Symbol()
	: m_bg(NULL)
{
}

s2::RenderReturn Symbol::DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr) const
{
	s2::RenderParams p = rp;
	if (spr) {
		p.mt = spr->GetLocalMat() * rp.mt;
		p.color = spr->GetColor() * rp.color;
	}

 	if (m_bg) {
		m_bg->DrawTree(nullptr, p, spr);
 	}
	if (ee::Config::Instance()->GetSettings().visible_shape)
	{
		for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
			m_bg_outline[i]->Draw(dlist, p);
		}
		s2::ShapeSymbol::DrawTree(nullptr, rp, spr);
	}

	return s2::RENDER_OK;
}

void Symbol::ReloadTexture() const
{
	if (m_bg) {
		m_bg->ReloadTexture();
	}
}

void Symbol::Traverse(ee::RefVisitor<ee::Shape>& visitor) const
{
	for (int i = 0, n = m_bg_outline.size(); i < n; ++i) {
		bool next;
		visitor.Visit(m_bg_outline[i], next);
		if (!next) return;
	}

	bool next;
	visitor.Visit(std::dynamic_pointer_cast<ee::Shape>(m_shape), next);
}

bool Symbol::Add(ee::ShapePtr& shape)
{
	SetShape(std::move(shape));
	return shape != NULL;
}

bool Symbol::Remove(const ee::ShapePtr& shape)
{
	if (m_shape == shape) {
		SetShape(NULL);
		return true;
	} else {
		return false;
	}
}

bool Symbol::Clear()
{
	bool ret = !m_bg_outline.empty() || m_shape != NULL;

	m_bg_outline.clear();

	m_shape.reset();

	return ret;
}

void Symbol::SetBG(const ee::SymPtr& bg)
{
	if (m_bg != bg) {
		LoadBGOutline(bg);
		LoadBGTriStrip(bg);
	}
	m_bg = bg;
}

void Symbol::StoreToFile(const char* filename) const
{
	FileIO::StoreToFile(filename, *dynamic_cast<ee::Shape*>(m_shape.get()), m_bg);
}

// sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
// {
// 	sm::rect b;
// 	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
// 		b.Combine(m_bg_outline[i]->GetBounding());
// 	}
// 	b.Combine(s2::ShapeSymbol::GetBounding(spr));
// 	return b;
// }

bool Symbol::LoadResources()
{
	if (m_filepath == ee::SYM_SHAPE_TAG) {
		return true;
	}

	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	Clear();
	m_shape = FileIO::LoadFromFile(m_filepath.c_str(), m_bg);

	return true;
}

void Symbol::LoadBGOutline(const ee::SymPtr& bg)
{
	m_bg_outline.clear();

	if (!bg) {
		return;
	}

	std::string filepath = ee::FileHelper::GetFilenameAddTag(
		bg->GetFilepath(), eimage::OUTLINE_FILE_TAG, "json");
	if (!ee::FileHelper::IsFileExist(filepath)) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	CU_VEC<sm::vec2> vertices;
	gum::JsonSerializer::Load(value["normal"], vertices);
	if (!vertices.empty()) {
		auto shape = std::make_unique<PolygonShape>(vertices);
		m_bg_outline.push_back(std::move(shape));
	}
}

void Symbol::LoadBGTriStrip(const ee::SymPtr& bg)
{
	m_bg_tri_strips.clear();

	std::string filepath = ee::FileHelper::GetFilenameAddTag(
		bg->GetFilepath(), eimage::TRI_STRIP_FILE_TAG, "json");
	if (!ee::FileHelper::IsFileExist(filepath)) {
		return;
	}
	
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value strip_val = value["strips"][i++];
	while (!strip_val.isNull()) {
		CU_VEC<sm::vec2> strip;
		gum::JsonSerializer::Load(strip_val, strip);
		m_bg_tri_strips.push_back(strip);
		strip_val = value["strip"][i++];
	}
}

ShapeType Symbol::GetShapeType() const
{
	if (!m_shape) {
		return ST_UNKNOWN;
	} else {
		return get_shape_type(dynamic_cast<ee::Shape*>(m_shape.get())->GetShapeDesc());
	}
}

}