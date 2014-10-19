#include "Symbol.h"
#include "Sprite.h"

#include <easy3d.h>

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
	if (!sprite) {
		return;
	}
	const Sprite* s = static_cast<const Sprite*>(sprite);

	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();

	mat4 mat = mat4(s->GetOri3().ToMatrix()) * 
		mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);
	e3d::DrawCube(mat, m_aabb, d2d::LIGHT_GREEN);

	shader->Lighting();
	shader->Draw(m_model, s->GetPos3(), s->GetOri3());
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return d2d::Rect(100, 100);
}

void Symbol::SetModel(e3d::IModel* model)
{
	if (m_model != model)
	{
		if (m_model) {
			m_model->release();
		}
		m_model = model;
		m_model->retain();
	}
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
  	m_model = new e3d::ModelObj(filepath.c_str(), 0.02f);
}

}