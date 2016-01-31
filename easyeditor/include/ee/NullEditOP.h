#ifndef _EASYEDITOR_NULL_EDIT_OP_H_
#define _EASYEDITOR_NULL_EDIT_OP_H_

#include "EditOP.h"

namespace ee
{

class NullEditOP : public EditOP
{
public:
	NullEditOP(wxWindow* wnd, EditPanelImpl* stage) 
		: EditOP(wnd, stage) 
	{}

}; // NullEditOP

}

#endif // _EASYEDITOR_NULL_EDIT_OP_H_