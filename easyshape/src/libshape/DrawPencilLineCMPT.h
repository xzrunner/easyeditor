#ifndef _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_
#define _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class DrawPencilLineCMPT : public d2d::AbstractEditCMPT
{
public:
	DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	float getSimplifyThreshold() const {
		return m_thresholdSlider->GetValue() * 0.1f;
	}

protected:
	virtual wxSizer* initLayout();

private:
	wxSlider* m_thresholdSlider;

}; // DrawPencilLineCMPT
	
}

#endif // _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_