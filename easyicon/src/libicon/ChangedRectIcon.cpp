#include "ChangedRectIcon.h"

#include <glue/JsonSerializer.h>

namespace eicon
{

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	glue::JsonSerializer::Load(value["begin"], m_begin);
	glue::JsonSerializer::Load(value["end"], m_end);
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	glue::JsonSerializer::Store(m_begin, value["begin"]);
	glue::JsonSerializer::Store(m_end, value["end"]);
}

}