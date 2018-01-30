#include "node2/CompBoundingBox.h"
#include "node2/SRT.h"

#include <guard/check.h>
#include <sprite2/OBB.h>

namespace n2
{

const char* const CompBoundingBox::TYPE_NAME = "bounding_box";

bool CompBoundingBox::StoreToJson(rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	return true;
}

void CompBoundingBox::LoadFromJson(const rapidjson::Value& val)
{
}

void CompBoundingBox::Build(const SRT& srt)
{
	m_bounding = std::make_unique<s2::OBB>();
	m_bounding->Build(m_size, srt.position, srt.angle, srt.scale, srt.shear, srt.offset);
}

const s2::BoundingBox& CompBoundingBox::GetBounding() const
{
	GD_ASSERT(m_bounding, "null bounding");
	return *m_bounding;
}

}