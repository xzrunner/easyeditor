#include "ee0/CompNodeEditor.h"

#include <string>

namespace ee0
{

const char* const CompNodeEditor::TYPE_NAME = "editor";

CompNodeEditor::CompNodeEditor()
{
	static size_t COUNT = 0;
	m_name = "node" + std::to_string(COUNT++);
}

bool CompNodeEditor::StoreToJson(rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("name", rapidjson::StringRef(m_name.c_str()), alloc);

	return true;
}

void CompNodeEditor::LoadFromJson(const rapidjson::Value& val)
{
	m_name = val["name"].GetString();
}

const std::string& CompNodeEditor::GetName() const
{
	return m_name;
}

void CompNodeEditor::SetName(const std::string& name)
{
	m_name = name;
}

}