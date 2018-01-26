#include "data/CompEditor.h"

#include <string>

namespace eone
{

const char* const CompEditor::TYPE_NAME = "editor";

CompEditor::CompEditor()
{
	static size_t COUNT = 0;
	m_name = "node" + std::to_string(COUNT++);
}

bool CompEditor::StoreToJson(rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	val.AddMember("name", rapidjson::StringRef(m_name.c_str()), alloc);

	return true;
}

void CompEditor::LoadFromJson(const rapidjson::Value& val)
{
	m_name = val["name"].GetString();
}

const std::string& CompEditor::GetName() const
{
	return m_name;
}

void CompEditor::SetName(const std::string& name)
{
	m_name = name;
}

}