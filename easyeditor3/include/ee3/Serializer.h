#pragma once

#include <rapidjson/document.h>

#include <string>

namespace n3 { class Camera; }

namespace ee3
{

class StagePage;

class Serializer
{
public:
	static void StoreToJson(const std::string& filepath, const StagePage* stage);
	static void LoadFroimJson(const std::string& filepath, StagePage* stage);

private:
	static rapidjson::Value StoreCamera(const n3::Camera& cam, rapidjson::MemoryPoolAllocator<>& alloc);
	static void LoadCamera(const rapidjson::Value& val, n3::Camera& cam);

}; // Serializer

}