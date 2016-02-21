#ifndef _EASYMODELING_FIXTURE_EDIT_CMPT_H_
#define _EASYMODELING_FIXTURE_EDIT_CMPT_H_

#include <ee/EditCMPT.h>

namespace ee { class PropertySettingPanel; }

namespace emodeling
{

class StagePanel;

class FixtureEditCmpt : public ee::EditCMPT
{
public:
	FixtureEditCmpt(wxWindow* parent, const std::string& name,
		StagePanel* editPanel, ee::PropertySettingPanel* property);

protected:
	virtual wxSizer* InitLayout();

}; // FixtureEditCmpt

}

#endif // _EASYMODELING_FIXTURE_EDIT_CMPT_H_
