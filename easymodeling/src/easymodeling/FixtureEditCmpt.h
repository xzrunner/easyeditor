
#ifndef EMODELING_FIXTURE_EDIT_CMPT_H
#define EMODELING_FIXTURE_EDIT_CMPT_H

#include <drag2d.h>

namespace emodeling
{
	class StagePanel;

	class FixtureEditCmpt : public d2d::AbstractEditCMPT
	{
	public:
		FixtureEditCmpt(wxWindow* parent, const wxString& name,
			StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel);

	protected:
		virtual wxSizer* initLayout();

	}; // FixtureEditCmpt
}

#endif // EMODELING_FIXTURE_EDIT_CMPT_H
