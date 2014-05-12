
#ifndef EMODELING_BODY_EDIT_CMPT_H
#define EMODELING_BODY_EDIT_CMPT_H

#include <drag2d.h>

namespace emodeling
{
	class StagePanel;

	class BodyEditCmpt : public d2d::AbstractEditCMPT
	{
	public:
		BodyEditCmpt(wxWindow* parent, const wxString& name,
			StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel);

	protected:
		virtual wxSizer* initLayout();

	}; // BodyEditCmpt
}

#endif // EMODELING_BODY_EDIT_CMPT_H
