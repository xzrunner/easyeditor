#include "ee3/Serializer.h"
#include "ee3/WxStagePage.h"
#include "ee3/WxStageCanvas.h"

#include <painting3/Camera.h>
#include <node0/SceneNode.h>
#include <node3/SerializeSystem.h>
#include <js/RapidJsonHelper.h>
#include <guard/check.h>

namespace ee3
{

void Serializer::StoreToJson(const std::string& filepath, const WxStagePage* stage)
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Value val_nodes;
	val_nodes.SetArray();

	auto& alloc = doc.GetAllocator();
	auto& nodes = stage->GetAllNodes();
	for (auto& node : nodes)
	{
		rapidjson::Value val_node;
		n3::SerializeSystem::StoreNodeToJson(node, val_node, alloc);
		val_nodes.PushBack(val_node, alloc);
	}
	doc.AddMember("nodes", val_nodes, alloc);

	auto canvas = std::dynamic_pointer_cast<const WxStageCanvas>(stage->GetImpl().GetCanvas());
	auto& cam = canvas->GetCamera();
	rapidjson::Value cam_val = StoreCamera(cam, alloc);
	doc.AddMember("camera", cam_val, alloc);

	js::RapidJsonHelper::WriteToFile(filepath.c_str(), doc);
}

void Serializer::LoadFroimJson(const std::string& filepath, WxStagePage* stage)
{
	rapidjson::Document doc;
	js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

	auto& nodes_val = doc["nodes"];
	for (auto& node_val : nodes_val.GetArray()) 
	{
		auto node = std::make_shared<n0::SceneNode>();
		n0::SceneNodePtr n3_node = node;
		n3::SerializeSystem::LoadNodeFromJson(n3_node, node_val);

		ee0::VariantSet vars;
		ee0::Variant var;
		var.m_type = ee0::VT_PVOID;
		var.m_val.pv = &node;
		vars.SetVariant("node", var);

		bool succ = stage->GetSubjectMgr().NotifyObservers(ee0::MSG_INSERT_SCENE_NODE, vars);
		GD_ASSERT(succ, "no MSG_INSERT_SCENE_NODE");
	}

	auto canvas = std::dynamic_pointer_cast<const WxStageCanvas>(stage->GetImpl().GetCanvas());
	auto& cam = const_cast<pt3::Camera&>(canvas->GetCamera());
	LoadCamera(doc["camera"], cam);
}

rapidjson::Value Serializer::StoreCamera(const pt3::Camera& cam, rapidjson::MemoryPoolAllocator<>& alloc)
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
	auto& up = cam.GetUpDir();
	up_val.SetObject();
	up_val.AddMember("x", up.x, alloc);
	up_val.AddMember("y", up.y, alloc);
	up_val.AddMember("z", up.z, alloc);
	val.AddMember("up", up_val, alloc);

	return val;
}

void Serializer::LoadCamera(const rapidjson::Value& val, pt3::Camera& cam)
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

}