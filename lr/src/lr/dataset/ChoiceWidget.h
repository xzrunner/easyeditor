#ifndef _LR_CHOICE_WIDGET_H_
#define _LR_CHOICE_WIDGET_H_

#include "DynamicWidget.h"

namespace lr
{

class ChoiceWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info);

	void SpecialLoad(const std::string& title, const std::string& key,
		const std::vector<std::string>& items, const std::string& default_item);

protected:
	virtual void Load(const Json::Value& value);

private:
	struct Item
	{
		std::string value;
		std::string title;
	};

	static const int MAX_ITEMS = 100;

private:
	int m_default;

	std::vector<Item> m_choices;

	wxChoice* m_ctrl;

}; // ChoiceWidget

}

#endif // _LR_CHOICE_WIDGET_H_