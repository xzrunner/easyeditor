#ifndef _LR_WIDGET_FACTORY_H_
#define _LR_WIDGET_FACTORY_H_

#include <json/json.h>

namespace lr
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

#endif // _LR_WIDGET_FACTORY_H_