#include "node2/CompBoundingBox.h"

#include <guard/check.h>
#include <painting2/OBB.h>
#include <painting2/SRT.h>

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

void CompBoundingBox::Build(const pt2::SRT& srt)
{
	m_bounding = std::make_unique<pt2::OBB>();
	m_bounding->Build(m_size, srt.position, srt.angle, srt.scale, srt.shear, srt.offset);
}

const pt2::BoundingBox& CompBoundingBox::GetBounding() const
{
	GD_ASSERT(m_bounding, "null bounding");
	return *m_bounding;
}

}