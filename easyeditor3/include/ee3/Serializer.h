#pragma once

#include <rapidjson/document.h>

#include <string>

namespace n3 { class Camera; }

namespace ee3
{

class WxStagePage;

class Serializer
{
public:
	static void StoreToJson(const std::string& filepath, const WxStagePage* stage);
	static void LoadFroimJson(const std::string& filepath, WxStagePage* stage);

private:
	static rapidjson::Value StoreCamera(const n3::Camera& cam, rapidjson::MemoryPoolAllocator<>& alloc);
	static void LoadCamera(const rapidjson::Value& val, n3::Camera& cam);

}; // Serializer

}