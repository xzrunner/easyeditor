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
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->Release();
	}
	m_shapes.clear();
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
	// todo
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		m_shapes[i]->draw(mt);
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::loadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
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

}