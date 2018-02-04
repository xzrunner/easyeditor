#pragma once

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <node0/NodeComponent.h>
#include <painting2/BoundingBox.h>

#include <memory>

namespace pt2 { class SRT; }

namespace n2
{

class CompBoundingBox : public n0::NodeComponent
{
public:
	CompBoundingBox(const sm::rect& size) : m_size(size) {}
	virtual ~CompBoundingBox() = default;

	virtual const char* Type() const override { return TYPE_NAME; }

	virtual bool StoreToJson(rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const rapidjson::Value& val) override;

	void Build(const pt2::SRT& srt);

	const pt2::BoundingBox& GetBounding() const;

	static const char* const TYPE_NAME;

private:
	sm::rect m_size;

	std::unique_ptr<pt2::BoundingBox> m_bounding = nullptr;

}; // CompBoundingBox

}