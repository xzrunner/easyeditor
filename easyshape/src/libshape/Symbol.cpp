#include "Symbol.h"
#include "FileIO.h"
#include "PolygonShape.h"

#include <easyimage.h>

#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SettingData.h>
#include <ee/Visitor.h>
#include <ee/Sprite.h>

#include <sprite2/RenderParams.h>
#include <gum/JsonSerializer.h>

#include <fstream>

namespace eshape
{

Symbol::Symbol()
	: m_bg(NULL)
{
}

Symbol::~Symbol()
{
	Clear();
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

 	if (m_bg) {
 		m_bg->Draw(p, spr);
 	}
	if (ee::Config::Instance()->GetSettings().visible_shape)
	{
		for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
			m_bg_outline[i]->Draw(p.mt);
		}
		for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
			m_shapes[i]->Draw(p.mt);
		}
	}
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	sm::rect b;
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		b.Combine(m_bg_outline[i]->GetBounding());
	}
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		b.Combine(m_shapes[i]->GetBounding());
	}
	return b;
}

void Symbol::ReloadTexture() const
{
	if (m_bg) {
		m_bg->ReloadTexture();
	}
}

void Symbol::Traverse(ee::Visitor<ee::Shape>& visitor) const
{
	for (int i = 0, n = m_bg_outline.size(); i < n; ++i) {
		bool next;
		visitor.Visit(m_bg_outline[i], next);
		if (!next) return;
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		bool next;
		visitor.Visit(m_shapes[i], next);
		if (!next) return;
	}
}

bool Symbol::Add(ee::Shape* shape)
{
	if (!shape) {
		return false;
	}

	shape->AddReference();
	m_shapes.push_back(shape);
	return true;
}

bool Symbol::Remove(ee::Shape* shape)
{
	if (!shape) {
		return false;
	}

	for (int i = 0, n = m_shapes.size(); i < n; ++i) 
	{
		if (m_shapes[i] == shape) 
		{
			m_shapes.erase(m_shapes.begin() + i);
			shape->RemoveReference();
			return true;
		}
	}

	return false;
}

bool Symbol::Clear()
{
	bool ret = !m_bg_outline.empty() || !m_shapes.empty();

	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		m_bg_outline[i]->RemoveReference();
	}
	m_bg_outline.clear();

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->RemoveReference();
	}
	m_shapes.clear();

	return ret;
}

void Symbol::SetBG(ee::Symbol* bg)
{
	if (m_bg != bg) {
		LoadBGOutline(bg);
		LoadBGTriStrip(bg);
	}
	cu::RefCountObjAssign(m_bg, bg);
}

void Symbol::StoreToFile(const char* filename) const
{
	std::vector<ee::Shape*> shapes(m_shapes);
	std::copy(m_bg_outline.begin(), m_bg_outline.end(), back_inserter(shapes));
	FileIO::StoreToFile(filename, shapes, m_bg);
}

void Symbol::LoadResources()
{
	Clear();
	FileIO::LoadFromFile(m_filepath.c_str(), m_shapes, m_bg);
}

void Symbol::LoadBGOutline(ee::Symbol* bg)
{
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		m_bg_outline[i]->RemoveReference();
	}
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

	std::vector<sm::vec2> vertices;
	gum::JsonSerializer::Load(value["normal"], vertices);
	if (!vertices.empty()) {
		ee::Shape* shape = new PolygonShape(vertices);
		m_bg_outline.push_back(shape);
	}
}

void Symbol::LoadBGTriStrip(ee::Symbol* bg)
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
		std::vector<sm::vec2> strip;
		gum::JsonSerializer::Load(strip_val, strip);
		m_bg_tri_strips.push_back(strip);
		strip_val = value["strip"][i++];
	}
}

ShapeType Symbol::GetShapeType() const
{
	if (m_shapes.empty()) {
		return ST_UNKNOWN;
	}

	ee::Shape* shape = m_shapes[0];
	return get_shape_type(shape->GetShapeDesc());
}

}