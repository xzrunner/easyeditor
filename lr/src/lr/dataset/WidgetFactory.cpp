#include "WidgetFactory.h"

#include "TextWidget.h"
#include "ChoiceWidget.h"
#include "ComboBoxWidget.h"
#include "CheckBoxWidget.h"



namespace lr
{

WidgetFactory* WidgetFactory::m_instance = NULL;

WidgetFactory* WidgetFactory::Instance()
{
	if (!m_instance) {
		m_instance = new WidgetFactory;
	}
	return m_instance;
}

DynamicWidget* WidgetFactory::Create(const Json::Value& val) const
{
	std::string type = val["type"].asString();
	DynamicWidget* w = NULL;
	if (type == "text") {
		w = new TextWidget;
	} else if (type == "choice") {
		w = new ChoiceWidget;
	} else if (type == "combo box") {
		w = new ComboBoxWidget;
	} else if (type == "check box") {
		w = new CheckBoxWidget;
	} else {
		throw ee::Exception("UnitCfg::LoadFromFile unknown type %s", type.c_str());
	}
	w->LoadFromFile(val);
	return w;
}

}