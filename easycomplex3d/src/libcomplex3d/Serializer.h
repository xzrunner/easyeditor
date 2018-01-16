#pragma once

#include <node3/INode.h>

#include <rapidjson/document.h>
#include <rapidjson/allocators.h>

#include <string>

namespace enode3d { class StagePanel; }
namespace n3 { class Surface; class Camera; }

namespace ecomplex3d
{

class Serializer
{
public:
	static void StoreToJson(const std::string& filepath, 
		const enode3d::StagePanel* stage);
	static void LoadFroimJson(const std::string& filepath,
		enode3d::StagePanel* stage);

private:
	static rapidjson::Value StoreNode(const n3::NodePtr& node, rapidjson::MemoryPoolAllocator<>& alloc);
	static n3::NodePtr LoadNode(const rapidjson::Value& val);

	static rapidjson::Value StoreCamera(const n3::Camera& cam, rapidjson::MemoryPoolAllocator<>& alloc);
	static void LoadCamera(const rapidjson::Value& val, n3::Camera& cam);

	static n3::ModelPtr LoadGeometric(n3::Surface* surface, const std::string& name);

}; // Serializer

}