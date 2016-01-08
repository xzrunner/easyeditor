#include "Symbol.h"

#include <easyshape.h>

namespace etexture
{

Symbol::Symbol()
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
	m_shapes.reserve(s.m_shapes.size());
	for (int i = 0, n = s.m_shapes.size(); i < n; ++i) {
		d2d::IShape* shape = s.m_shapes[i];
		m_shapes.push_back(shape);
		shape->Retain();
	}
}

Symbol::~Symbol()
{
	Clear();
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->ReloadTexture();
	}
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::ISprite* spr, const d2d::ISprite* root) const
{
	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->SetSpriteColor(color);
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Draw(mt, color);
	}
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	d2d::Rect rect;
	for (size_t i = 0, n = m_shapes.size(); i < n; ++i) {
		rect.combine(m_shapes[i]->GetRect());
	}
	return rect;
}

void Symbol::LoadResources()
{
	Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(m_filepath);

	int i = 0;
	Json::Value shape_val = value["shapes"][i++];
	while (!shape_val.isNull()) {
		d2d::IShape* shape = libshape::FileIO::LoadShape(dir, shape_val);
		m_shapes.push_back(shape);
		shape_val = value["shapes"][i++];
	}
}

void Symbol::Clear()
{
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Release();
	}
	m_shapes.clear();
}

}