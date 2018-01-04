#include "Symbol.h"
#include "Sprite.h"

#include <ee/FileHelper.h>

#include <node3/ModelObj.h>
#include <node3/ObjectModel.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace ecomplex3d
{

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

  	Json::Value value;
  	Json::Reader reader;
  	std::locale::global(std::locale(""));
  	std::ifstream fin(m_filepath.c_str());
  	std::locale::global(std::locale("C"));
  	reader.parse(fin, value);
  	fin.close();
  
  	wxString dir = ee::FileHelper::GetFileDir(m_filepath);
  	std::string filepath = ee::FileHelper::GetAbsolutePath(dir.ToStdString(), value["filepath"].asString());
	auto obj = std::make_unique<n3::ModelObj>(filepath.c_str(), 0.02f);

	auto obj_model = std::make_shared<n3::ObjectModel>();
//	obj_model->SetModel(std::make_unique<n3::Model>(obj.get()));

	SetModel(obj_model);

	return true;
}

}