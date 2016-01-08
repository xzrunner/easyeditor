#ifndef _LR_TEXT_WIDGET_H_
#define _LR_TEXT_WIDGET_H_

#include "DynamicWidget.h"

namespace lr
{

class TextWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const;

	virtual bool IsChanged() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info);

protected:
	virtual void Load(const Json::Value& value);
	
private:
	std::string m_default;

	wxTextCtrl* m_ctrl;

	std::string m_ori_val;

}; // TextWidget

}

#endif // _LR_TEXT_WIDGET_H_