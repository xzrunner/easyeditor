#pragma once

#include <node0/NodeComponent.h>
#include <painting2/Texture.h>

#include <memory>

namespace n2
{

class CompImage : public n0::NodeComponent
{
public:
	virtual const char* Type() const override { return TYPE_NAME; }

	virtual bool StoreToJson(rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const rapidjson::Value& val) override;

	void SetTexture(const std::shared_ptr<pt2::Texture>& tex) { m_tex = tex; }
	const std::shared_ptr<pt2::Texture>& GetTexture() const { return m_tex; }

	static const char* const TYPE_NAME;

private:
	std::shared_ptr<pt2::Texture> m_tex = nullptr;

}; // CompImage

}