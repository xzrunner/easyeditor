#include "DynamicWidget.h"

namespace lr
{

void DynamicWidget::LoadFromFile(const Json::Value& value)
{
	m_title = value["title"].asString();
	m_key = value["key"].asString();
	m_special = false;
	if (!value["special"].isNull()) {
		m_special = value["special"].asBool();
	}

	Load(value);
}

}