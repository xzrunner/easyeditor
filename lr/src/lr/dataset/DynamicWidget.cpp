#include "DynamicWidget.h"
#include "DynamicInfo.h"

namespace lr
{

DynamicWidget::DynamicWidget()
	: m_special(false)
{
}

void DynamicWidget::LoadFromFile(const Json::Value& value)
{
	m_title = value["title"].asString();
	m_key = value["key"].asString();
	if (!value["special"].isNull()) {
		m_special = value["special"].asBool();
	}

	Load(value);
}

}