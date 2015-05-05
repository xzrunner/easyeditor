#ifndef _DRAG2D_ONE_FLOAT_VALUE_CMPT_H_
#define _DRAG2D_ONE_FLOAT_VALUE_CMPT_H_

#include "OneFloatValue.h"

#include "component/AbstractEditCMPT.h"

namespace d2d
{

class OneFloatValueCMPT : public AbstractEditCMPT, public OneFloatValue
{
public:
	OneFloatValueCMPT(wxWindow* parent, const wxString& name,
		EditPanel* editPanel, const std::string& title, 
		int min = 0, int max = 100, int value = 0, float scale = 1);

	//
	// interface OneFloatValue
	//
	virtual float GetScope() const;

protected:
	virtual wxSizer* initLayout();

private:
	std::string m_title;

	int m_min, m_max;
	int m_value;

	float m_scale;

	wxSlider* m_slider;

}; // OneFloatValueCMPT

}

#endif // _DRAG2D_ONE_FLOAT_VALUE_CMPT_H_