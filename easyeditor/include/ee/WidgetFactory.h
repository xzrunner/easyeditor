#ifndef _EASYEDITOR_WIDGET_FACTORY_H_
#define _EASYEDITOR_WIDGET_FACTORY_H_

#include <json/json.h>

namespace ee
{

class DynamicWidget;

class WidgetFactory
{
public:
	DynamicWidget* Create(const Json::Value& val) const;

	static WidgetFactory* Instance();

private:
	WidgetFactory() {}

private:
	static WidgetFactory* m_instance;

}; // WidgetFactory

}

#endif // _EASYEDITOR_WIDGET_FACTORY_H_