#include "Symbol.h"
#include "Sprite.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace eanim3d
{

Symbol::Symbol()
	: m_model(NULL)
{
}

Symbol::~Symbol()
{
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	if (!spr) {
		return;
	}
	const Sprite* s = dynamic_cast<const Sprite*>(spr);

	e3d::ShaderMgr* mgr = e3d::ShaderMgr::Instance();

	sm::mat4 mat = sm::mat4(s->GetOri3().ToMatrix()) * 
		sm::mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);

	// 	e3d::DrawCube(mat, m_aabb, ee::BLACK);

	mgr->Model();
	mgr->DrawModel(m_model, mat);
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	return sm::rect(100, 100);
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

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);
	std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["filepath"].asString());
	m_model = new e3d::ModelObj(filepath.c_str(), 0.02f);
}

void Symbol::SetModel(e3d::IModel* model)
{
	if (m_model != model)
	{
		if (m_model) {
			m_model->RemoveReference();
		}
		m_model = model;
		m_model->AddReference();
	}
}

}