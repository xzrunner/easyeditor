#include "ee3/NodeSymbol.h"

#include <ee/FileHelper.h>

#include <gum/FilepathHelper.h>
#include <node3/AssimpHelper.h>
#include <node3/AABB.h>
#include <node3/CompModel.h>
#include <node3/CompAABB.h>
#include <js/RapidJsonHelper.h>

namespace ee3
{

sm::rect NodeSymbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool NodeSymbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	auto ext = gum::FilepathHelper::Extension(m_filepath);
	if (ext == ".json")
	{
		rapidjson::Document doc;
		js::RapidJsonHelper::ReadFromFile(m_filepath.c_str(), doc);
		auto filepath = doc["filepath"].GetString();

		wxString dir = ee::FileHelper::GetFileDir(m_filepath);
		std::string absolute_path = ee::FileHelper::GetAbsolutePath(dir.ToStdString(), filepath);
		return LoadModelFromFile(absolute_path);
	}
	else if (ext == ".x")
	{
		return LoadModelFromFile(m_filepath);
	}

	return false;
}

bool NodeSymbol::LoadModelFromFile(const std::string& filepath)
{
	n3::AABB aabb;
	auto model = n3::AssimpHelper::Load(filepath, aabb);
	if (!model) {
		return false;
	}

	auto node = std::make_shared<n3::SceneNode>();

	auto& cmodel = node->AddComponent<n3::CompModel>();
	cmodel.SetModel(model);

	auto& caabb = node->AddComponent<n3::CompAABB>();
	caabb.SetAABB(aabb);

	SetNode(node);

	return true;
}

}