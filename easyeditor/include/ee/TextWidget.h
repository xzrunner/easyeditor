#ifndef _EASYEDITOR_TEXT_WIDGET_H_
#define _EASYEDITOR_TEXT_WIDGET_H_

#include "DynamicWidget.h"

namespace ee
{

class TextWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const override;

	virtual bool IsChanged() const override;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info) override;

protected:
	virtual void Load(const Json::Value& value) override;
	
private:
	std::string m_default;

	wxTextCtrl* m_ctrl;

	std::string m_ori_val;

}; // TextWidget

}

#endif // _EASYEDITOR_TEXT_WIDGET_H_