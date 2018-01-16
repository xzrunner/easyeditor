#include "Serializer.h"

#include <ee/FileHelper.h>

#include <easynode3d/StagePanel.h>
#include <easynode3d/StageCanvas.h>
#include <easynode3d/Symbol.h>
#include <easynode3d/NodeFactory.h>

#include <guard/check.h>
#include <rapidjson/document.h>
#include <js/RapidJsonHelper.h>
#include <gum/FilepathHelper.h>
#include <node3/ModelParametric.h>
#include <node3/ObjectModel.h>
#include <node3/SurfaceFactory.h>

namespace ecomplex3d
{

void Serializer::StoreToJson(const std::string& filepath, const enode3d::StagePanel* stage)
{
	rapidjson::Document doc;
	doc.SetObject();

	auto& alloc = doc.GetAllocator();

	rapidjson::Value nodes_val;
	auto& nodes = stage->GetAllNodes();
	nodes_val.SetArray();
	for (auto& node : nodes)
	{
		rapidjson::Value node_val = StoreNode(node, alloc);
		nodes_val.PushBack(node_val, alloc);
	}
	doc.AddMember("nodes", nodes_val, alloc);

	auto canvas = dynamic_cast<const enode3d::StageCanvas*>(stage->GetCanvas());
	auto& cam = canvas->GetCamera();
	rapidjson::Value cam_val = StoreCamera(cam, alloc);
	doc.AddMember("camera", cam_val, alloc);

	js::RapidJsonHelper::WriteToFile(filepath.c_str(), doc);
}

void Serializer::LoadFroimJson(const std::string& filepath, enode3d::StagePanel* stage)
{
	rapidjson::Document doc;
	js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

	auto& nodes_val = doc["nodes"];
	rapidjson::Value::ConstValueIterator itr = nodes_val.Begin();
	for ( ; itr != nodes_val.End(); ++itr) {
		auto node = LoadNode(*itr);
		GD_ASSERT(node != nullptr, "unknown node.");
		stage->InsertNode(node);
	}

	auto canvas = dynamic_cast<enode3d::StageCanvas*>(stage->GetCanvas());
	auto& cam = canvas->GetCamera();
	auto& cam_val = doc["camera"];
	LoadCamera(cam_val, cam);
}

rapidjson::Value Serializer::StoreNode(const n3::NodePtr& node,
                                       rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value val;
	val.SetObject();

	rapidjson::Value path_val(node->GetModel()->GetFilepath().c_str(), alloc);
	val.AddMember("filepath", path_val, alloc);

	rapidjson::Value pos_val;
	auto& pos = node->GetPos();
	pos_val.SetObject();
	pos_val.AddMember("x", pos.x, alloc);
	pos_val.AddMember("y", pos.y, alloc);
	pos_val.AddMember("z", pos.z, alloc);
	val.AddMember("pos", pos_val, alloc);

	rapidjson::Value angle_val;
	auto& angle = node->GetAngle();
	angle_val.SetObject();
	angle_val.AddMember("x", angle.x, alloc);
	angle_val.AddMember("y", angle.y, alloc);
	angle_val.AddMember("z", angle.z, alloc);
	angle_val.AddMember("w", angle.w, alloc);
	val.AddMember("angle", angle_val, alloc);

	return val;
}

n3::NodePtr Serializer::LoadNode(const rapidjson::Value& val)
{
	n3::ModelPtr model = nullptr;

	std::string filepath = val["filepath"].GetString();
	auto ext = gum::FilepathHelper::Extension(filepath);
	if (ext == ".geo")
	{
		auto name = ee::FileHelper::GetFilename(filepath.c_str());
		n3::Surface* surface = n3::SurfaceFactory::Create(name);
		if (!surface) {
			return nullptr;
		}
		model = LoadGeometric(surface, name.c_str());
	}

	if (!model) {
		return nullptr;
	}

	// todo: insert to library list
	auto sym = std::make_shared<enode3d::Symbol>();
	sym->SetName("Geo");
	sym->SetModel(model);
	sym->SetFilepath(filepath);

	auto node = enode3d::NodeFactory::Instance()->Create(sym);
	if (!node) {
		return nullptr;
	}

	sm::vec3 pos;
	auto& pos_val = val["pos"];
	pos.x = pos_val["x"].GetFloat();
	pos.y = pos_val["y"].GetFloat();
	pos.z = pos_val["z"].GetFloat();
	node->SetPos(pos);

	sm::Quaternion angle;
	auto& angle_val = val["angle"];
	angle.x = angle_val["x"].GetFloat();
	angle.y = angle_val["y"].GetFloat();
	angle.z = angle_val["z"].GetFloat();
	angle.w = angle_val["w"].GetFloat();
	node->SetAngle(angle);

	return node;
}

rapidjson::Value Serializer::StoreCamera(const n3::Camera& cam, rapidjson::MemoryPoolAllocator<>& alloc)
{
	rapidjson::Value val;
	val.SetObject();

	rapidjson::Value pos_val;
	auto& pos = cam.GetPos();
	pos_val.SetObject();
	pos_val.AddMember("x", pos.x, alloc);
	pos_val.AddMember("y", pos.y, alloc);
	pos_val.AddMember("z", pos.z, alloc);
	val.AddMember("pos", pos_val, alloc);

	rapidjson::Value target_val;
	auto& target = cam.GetTarget();
	target_val.SetObject();
	target_val.AddMember("x", target.x, alloc);
	target_val.AddMember("y", target.y, alloc);
	target_val.AddMember("z", target.z, alloc);
	val.AddMember("target", target_val, alloc);

	rapidjson::Value up_val;
	auto& up = cam.GetUp();
	up_val.SetObject();
	up_val.AddMember("x", up.x, alloc);
	up_val.AddMember("y", up.y, alloc);
	up_val.AddMember("z", up.z, alloc);
	val.AddMember("up", up_val, alloc);

	return val;
}

void Serializer::LoadCamera(const rapidjson::Value& val, n3::Camera& cam)
{
	sm::vec3 pos;
	auto& pos_val = val["pos"];
	pos.x = pos_val["x"].GetFloat();
	pos.y = pos_val["y"].GetFloat();
	pos.z = pos_val["z"].GetFloat();

	sm::vec3 target;
	auto& target_val = val["target"];
	target.x = target_val["x"].GetFloat();
	target.y = target_val["y"].GetFloat();
	target.z = target_val["z"].GetFloat();

	sm::vec3 up;
	auto& up_val = val["up"];
	up.x = up_val["x"].GetFloat();
	up.y = up_val["y"].GetFloat();
	up.z = up_val["z"].GetFloat();

	cam.Reset(pos, target, up);
}

n3::ModelPtr Serializer::LoadGeometric(n3::Surface* surface, const std::string& name)
{
	n3::AABB aabb;
	auto model = std::unique_ptr<n3::Model>(new n3::ModelParametric(surface, aabb));
	auto obj_model = std::make_shared<n3::ObjectModel>();
	obj_model->SetModel(model);
	obj_model->SetAABB(aabb);

	std::string filepath = name + ".geo";
	obj_model->SetFilepath(filepath);

	return obj_model;
}

}