#include "node2/CompImage.h"

namespace n2
{

const char* const CompImage::TYPE_NAME = "image";

bool CompImage::StoreToJson(rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	return false;
}

void CompImage::LoadFromJson(const rapidjson::Value& val)
{
}

}