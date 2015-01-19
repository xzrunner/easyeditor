#include "Symbol.h"
#include "EJSprite.h"
#include "PackageMgr.h"

namespace eejoy2d
{

Symbol::Symbol()
	: m_spr(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
	d2d::obj_assign((d2d::Object*&)m_spr, s.m_spr);
}

Symbol::~Symbol()
{
	PackageMgr::Instance()->Remove(m_filepath.ToStdString());

	if (m_spr) {
		m_spr->Release();
	}
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	if (m_spr) {
		m_spr->Draw(0, 0);
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

	std::string filepath = value["ep file"].asString();
	std::string name = value["export name"].asString();

	ej_package* pkg = PackageMgr::Instance()->Fetch(filepath);
	assert(pkg);
	m_spr = new eejoy2d::EJSprite(pkg, name.c_str());
}

}