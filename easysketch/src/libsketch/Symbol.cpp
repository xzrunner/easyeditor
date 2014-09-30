#include "Symbol.h"
#include "ModelObj.h"

namespace libsketch
{

Symbol::Symbol()
	: m_model(NULL)
{
}

Symbol::~Symbol()
{
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
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	d2d::ShaderNew* shader = d2d::ShaderNew::Instance();
	shader->lighting();
	shader->Draw(m_model);
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return d2d::Rect(100, 100);
}

void Symbol::loadResources()
{
	if (m_model) {
		delete m_model;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FilenameTools::getFileDir(m_filepath);
	std::string filepath = d2d::FilenameTools::getAbsolutePath(dir, value["filepath"].asString());
	m_model = new z3d::ModelObj(filepath.c_str(), 0.05f);
}

}