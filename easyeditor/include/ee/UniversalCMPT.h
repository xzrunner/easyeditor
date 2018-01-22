#ifndef _EASYEDITOR_UNIVERSAL_CMPT_H_
#define _EASYEDITOR_UNIVERSAL_CMPT_H_

#include "EditCMPT.h"

#include <memory>

namespace ee
{

class EditOP;

class UniversalCMPT : public EditCMPT
{
public:
	UniversalCMPT(wxWindow* parent, const std::string& name,
		EditPanelImpl* stage, const std::shared_ptr<EditOP>& editop);

protected:
	virtual wxSizer* InitLayout() override;

}; // UniversalCMPT

}

#endif // _EASYEDITOR_UNIVERSAL_CMPT_H_