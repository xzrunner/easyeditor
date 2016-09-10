#include "ChangedRectIcon.h"

#include <gum/JsonSerializer.h>

namespace eicon
{

void ChangedRectIcon::LoadFromFile(const Json::Value& value)
{
	gum::JsonSerializer::Load(value["begin"], m_begin);
	gum::JsonSerializer::Load(value["end"], m_end);
}

void ChangedRectIcon::StoreToFile(Json::Value& value) const
{
	gum::JsonSerializer::Store(m_begin, value["begin"]);
	gum::JsonSerializer::Store(m_end, value["end"]);
}

}