#ifndef _EASYEDITOR_ONE_FLOAT_VALUE_CMPT_H_
#define _EASYEDITOR_ONE_FLOAT_VALUE_CMPT_H_

#include "OneFloatValue.h"
#include "EditCMPT.h"

#include <wx/wx.h>

namespace ee
{

class OneFloatValueCMPT : public EditCMPT, public OneFloatValue
{
public:
 	OneFloatValueCMPT(wxWindow* parent, const std::string& name,
 		EditPanelImpl* stage, const std::string& title, 
 		int min = 0, int max = 100, int value = 0, float scale = 1);

	//
	// interface OneFloatValue
	//
	virtual float GetValue() const;

protected:
	virtual wxSizer* InitLayout();

private:
	std::string m_title;

	int m_min, m_max;
	int m_value;

	float m_scale;

	wxSlider* m_slider;

}; // OneFloatValueCMPT

}

#endif // _EASYEDITOR_ONE_FLOAT_VALUE_CMPT_H_