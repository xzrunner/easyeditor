#pragma once

#include <rapidjson/document.h>

#include <string>

namespace n3 { class Camera; }

namespace eone
{

class StagePanel;

class Serializer
{
public:
	static void StoreToJson(const std::string& filepath, const StagePanel* stage);
	static void LoadFroimJson(const std::string& filepath, StagePanel* stage);

private:
	static rapidjson::Value StoreCamera(const n3::Camera& cam, rapidjson::MemoryPoolAllocator<>& alloc);
	static void LoadCamera(const rapidjson::Value& val, n3::Camera& cam);

}; // Serializer

}