#ifndef _EASYEDITOR_UNIVERSAL_CMPT_H_
#define _EASYEDITOR_UNIVERSAL_CMPT_H_

#include "EditCMPT.h"

namespace ee
{

class UniversalCMPT : public EditCMPT
{
public:
	UniversalCMPT(wxWindow* parent, const std::string& name,
		EditPanelImpl* stage, EditOP* editOP);

protected:
	virtual wxSizer* InitLayout();

}; // UniversalCMPT

}

#endif // _EASYEDITOR_UNIVERSAL_CMPT_H_