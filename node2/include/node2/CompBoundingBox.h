#pragma once

#include <SM_Vector.h>
#include <node0/NodeComponent.h>
#include <sprite2/BoundingBox.h>

#include <memory>

namespace n2
{

class SRT;

class CompBoundingBox : public n0::NodeComponent
{
public:
	CompBoundingBox(const sm::rect& size) : m_size(size) {}

	virtual const char* Type() const override { return TYPE_NAME; }

	virtual bool StoreToJson(rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const rapidjson::Value& val) override;

	void Build(const SRT& srt);

	const s2::BoundingBox& GetBounding() const;

	static const char* const TYPE_NAME;

private:
	sm::rect m_size;

	std::unique_ptr<s2::BoundingBox> m_bounding = nullptr;

}; // CompBoundingBox

}