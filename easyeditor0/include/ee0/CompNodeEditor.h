#pragma once

#include <node0/NodeComponent.h>

namespace ee0
{

class CompNodeEditor : public n0::NodeComponent
{
public:
	CompNodeEditor();

	virtual const char* Type() const override { return TYPE_NAME; }

	virtual bool StoreToJson(rapidjson::Value& val,
		rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const rapidjson::Value& val) override;

	const std::string& GetName() const;
	void SetName(const std::string& name);

	static const char* const TYPE_NAME;

private:
	std::string m_name;

}; // CompNodeEditor

}