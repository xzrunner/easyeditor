#include "Symbol.h"
#include "FileIO.h"

#include "PolygonShape.h"

#include <easyimage.h>

namespace libshape
{

Symbol::Symbol()
	: m_bg(NULL)
{
}

Symbol::Symbol(const Symbol& symbol)
{
	m_filepath = symbol.m_filepath;

	m_bg = symbol.m_bg;
	if (m_bg) {
		m_bg->Retain();
	}
	m_bg_outline.reserve(symbol.m_bg_outline.size());
	for (size_t i = 0, n = symbol.m_bg_outline.size(); i < n; ++i) {
		m_bg_outline.push_back(symbol.m_bg_outline[i]->clone());
	}
	
	m_shapes.reserve(symbol.m_shapes.size());
	for (size_t i = 0, n = symbol.m_shapes.size(); i < n; ++i) {
		m_shapes.push_back(symbol.m_shapes[i]->clone());
	}
}

Symbol::~Symbol()
{
	Clear();
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this); 
}

void Symbol::reloadTexture() const
{
	if (m_bg) {
		m_bg->reloadTexture();
	}
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
 	if (m_bg) {
 		m_bg->draw(mt, mul, add, r_trans, g_trans, b_trans, sprite);
 	}
	if (d2d::Config::Instance()->GetSettings().visible_shape)
	{
		for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
			m_bg_outline[i]->draw(mt);
		}
		for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
			m_shapes[i]->draw(mt);
		}
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	d2d::Rect rect;
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		rect.combine(m_bg_outline[i]->getRect());
	}
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		rect.combine(m_shapes[i]->getRect());
	}
	return rect;
}

void Symbol::Traverse(d2d::IVisitor& visitor) const
{
	for (int i = 0, n = m_bg_outline.size(); i < n; ++i) {
		bool hasNext;
		visitor.visit(m_bg_outline[i], hasNext);
		if (!hasNext) return;
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		bool hasNext;
		visitor.visit(m_shapes[i], hasNext);
		if (!hasNext) return;
	}
}

void Symbol::Add(d2d::IShape* shape)
{
	if (shape) {
		shape->Retain();
		m_shapes.push_back(shape);
	}
}

void Symbol::Remove(d2d::IShape* shape)
{
	if (!shape) {
		return;
	}

	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		if (m_shapes[i] == shape) {
			m_shapes.erase(m_shapes.begin() + i);
			shape->Release();
			break;
		}
	}
}

void Symbol::Clear()
{
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		m_bg_outline[i]->Release();
	}
	m_bg_outline.clear();

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Release();
	}
	m_shapes.clear();
}

void Symbol::SetBG(d2d::ISymbol* bg)
{
	if (m_bg != bg) {
		LoadBGOutline(bg);
		LoadBGTriStrip(bg);
	}
	d2d::obj_assign((d2d::Object*&)m_bg, bg);
}

void Symbol::StoreToFile(const char* filename) const
{
	std::vector<d2d::IShape*> shapes(m_shapes);
	std::copy(m_bg_outline.begin(), m_bg_outline.end(), back_inserter(shapes));
	FileIO::StoreToFile(filename, shapes, m_bg);
}

void Symbol::loadResources()
{
	Clear();
	FileIO::LoadFromFile(m_filepath, m_shapes, m_bg);
}

void Symbol::LoadBGOutline(d2d::ISymbol* bg)
{
	for (size_t i = 0, n = m_bg_outline.size(); i < n; ++i) {
		m_bg_outline[i]->Release();
	}
	m_bg_outline.clear();

	if (!bg) {
		return;
	}

	wxString filepath = d2d::FilenameTools::getFilenameAddTag(
		bg->getFilepath(), eimage::OUTLINE_FILE_TAG, "json");
	if (!d2d::FilenameTools::isExist(filepath)) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::vector<d2d::Vector> vertices;
	d2d::JsonIO::Load(value["normal"], vertices);
	if (!vertices.empty()) {
		d2d::IShape* shape = new PolygonShape(vertices);
		m_bg_outline.push_back(shape);
	}
}

void Symbol::LoadBGTriStrip(d2d::ISymbol* bg)
{
	m_bg_tri_strips.clear();

	wxString filepath = d2d::FilenameTools::getFilenameAddTag(
		bg->getFilepath(), eimage::TRI_STRIP_FILE_TAG, "json");
	if (!d2d::FilenameTools::isExist(filepath)) {
		return;
	}
	
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value strip_val = value["strips"][i++];
	while (!strip_val.isNull()) {
		std::vector<d2d::Vector> strip;
		d2d::JsonIO::Load(strip_val, strip);
		m_bg_tri_strips.push_back(strip);
		strip_val = value["strip"][i++];
	}
}

ShapeType Symbol::GetShapeType() const
{
	if (m_shapes.empty()) {
		return ST_UNKNOWN;
	}

	d2d::IShape* shape = m_shapes[0];
	return get_shape_type(shape->GetShapeDesc());
}

}