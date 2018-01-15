#include "Symbol.h"
#include "Sprite.h"

#include <ee/FileHelper.h>

#include <node3/ModelObj.h>
#include <node3/ObjectModel.h>
#include <node3/AssimpHelper.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace enode3d
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

	auto ext = gum::FilepathHelper::Extension(m_filepath);
	if (ext == ".json")
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(m_filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		wxString dir = ee::FileHelper::GetFileDir(m_filepath);
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir.ToStdString(), value["filepath"].asString());
		return LoadModelFromFile(filepath);
	}
	else if (ext == ".x")
	{
		return LoadModelFromFile(m_filepath);
	}

	return false;
}

bool Symbol::LoadModelFromFile(const std::string& filepath)
{
	n3::AABB aabb;
	auto model = std::unique_ptr<n3::Model>(
		n3::AssimpHelper::Load(filepath, aabb));
	if (!model) {
		return false;
	}

	auto obj_model = std::make_shared<n3::ObjectModel>();
	obj_model->SetModel(model);
	obj_model->SetAABB(aabb);

	SetModel(obj_model);

	return true;
}

}