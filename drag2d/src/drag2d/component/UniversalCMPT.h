#ifndef _DRAG2D_UNIVERSAL_CMPT_H_
#define _DRAG2D_UNIVERSAL_CMPT_H_

#include "AbstractEditCMPT.h"

namespace d2d
{

class UniversalCMPT : public AbstractEditCMPT
{
public:
	UniversalCMPT(wxWindow* parent, const wxString& name,
		EditPanelImpl* stage, AbstractEditOP* editOP);

protected:
	virtual wxSizer* initLayout();

}; // UniversalCMPT

}

#endif // _DRAG2D_UNIVERSAL_CMPT_H_