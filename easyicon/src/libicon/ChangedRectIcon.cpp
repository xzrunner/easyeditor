#include "ChangedRectIcon.h"

#include <ee/JsonSerializer.h>

namespace eicon
{

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	ee::JsonSerializer::Load(value["begin"], m_begin);
	ee::JsonSerializer::Load(value["end"], m_end);
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	ee::JsonSerializer::Store(m_begin, value["begin"]);
	ee::JsonSerializer::Store(m_end, value["end"]);
}

}