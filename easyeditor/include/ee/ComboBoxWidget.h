#ifndef _EASYEDITOR_COMBO_BOX_WIDGET_H_
#define _EASYEDITOR_COMBO_BOX_WIDGET_H_

#include "DynamicWidget.h"

namespace ee
{

class ComboBoxWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const override;

	virtual bool IsChanged() const override;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info) override;

protected:
	virtual void Load(const Json::Value& value) override;

private:
	void OnText(wxCommandEvent& event);

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

	wxComboBox* m_ctrl;

	int m_ori_val;

}; // ComboBoxWidget

}

#endif // _EASYEDITOR_COMBO_BOX_WIDGET_H_