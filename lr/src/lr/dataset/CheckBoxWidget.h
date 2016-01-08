#ifndef _LR_CHECK_BOX_WIDGET_H_
#define _LR_CHECK_BOX_WIDGET_H_

#include "DynamicWidget.h"

namespace lr
{

class CheckBoxWidget : public DynamicWidget
{
public:
	virtual std::string GetValue() const;

	virtual bool IsChanged() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info);

protected:
	virtual void Load(const Json::Value& value);
	
private:
	bool m_default;	
	bool m_has_default;

	wxCheckBox* m_ctrl;

	bool m_ori_val;

}; // CheckBoxWidget

}

#endif // _LR_CHECK_BOX_WIDGET_H_