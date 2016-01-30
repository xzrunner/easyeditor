#include "Symbol.h"
#include "Sprite.h"

namespace eanim3d
{

Symbol::Symbol()
	: m_model(NULL)
{
}

Symbol::~Symbol()
{
}

void Symbol::ReloadTexture() const
{
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	if (!spr) {
		return;
	}
	const Sprite* s = static_cast<const Sprite*>(spr);

	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();

	mat4 mat = mat4(s->GetOri3().ToMatrix()) * 
		mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);

	// 	e3d::DrawCube(mat, m_aabb, d2d::BLACK);

	shader->Model();
	shader->DrawModel(m_model, mat);
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite) const
{
	return d2d::Rect(100, 100);
}

void Symbol::LoadResources()
{
	if (m_model) {
		delete m_model;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FileHelper::GetFileDir(m_filepath);
	std::string filepath = d2d::FileHelper::GetAbsolutePath(dir, value["filepath"].asString());
	m_model = new e3d::ModelObj(filepath.c_str(), 0.02f);
}

void Symbol::SetModel(e3d::IModel* model)
{
	if (m_model != model)
	{
		if (m_model) {
			m_model->Release();
		}
		m_model = model;
		m_model->Retain();
	}
}

}