#ifndef _EASYMODELING_BODY_EDIT_CMPT_H_
#define _EASYMODELING_BODY_EDIT_CMPT_H_

#include <ee/EditCMPT.h>

namespace ee { class PropertySettingPanel; }

namespace emodeling
{

class StagePanel;

class BodyEditCmpt : public ee::EditCMPT
{
public:
	BodyEditCmpt(wxWindow* parent, const std::string& name,
		StagePanel* editPanel, ee::PropertySettingPanel* property);

protected:
	virtual wxSizer* InitLayout();

}; // BodyEditCmpt

}

#endif // _EASYMODELING_BODY_EDIT_CMPT_H_
