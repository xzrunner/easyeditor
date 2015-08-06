#ifndef _DRAG2D_NULL_EDIT_OP_H_
#define _DRAG2D_NULL_EDIT_OP_H_

#include "AbstractEditOP.h"

namespace d2d
{

class NullEditOP : public AbstractEditOP
{
public:
	NullEditOP(wxWindow* wnd, EditPanelImpl* stage) 
		: AbstractEditOP(wnd, stage) 
	{}

}; // NullEditOP

}

#endif // _DRAG2D_NULL_EDIT_OP_H_