#ifndef _LIBSHAPE_DRAW_PENCIL_POLYGON_CMPT_H_
#define _LIBSHAPE_DRAW_PENCIL_POLYGON_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class DrawPencilPolygonCMPT : public d2d::OneFloatValueCMPT
{
public:
	DrawPencilPolygonCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

}; // DrawPencilPolygonCMPT
	
}

#endif // _LIBSHAPE_DRAW_PENCIL_POLYGON_CMPT_H_