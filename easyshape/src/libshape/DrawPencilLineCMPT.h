#ifndef _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_
#define _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class DrawPencilLineCMPT : public d2d::OneFloatValueCMPT
{
public:
	DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

}; // DrawPencilLineCMPT
	
}

#endif // _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_