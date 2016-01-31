
#ifndef EMODELING_BODY_EDIT_CMPT_H
#define EMODELING_BODY_EDIT_CMPT_H



namespace emodeling
{
	class StagePanel;

	class BodyEditCmpt : public ee::EditCMPT
	{
	public:
		BodyEditCmpt(wxWindow* parent, const wxString& name,
			StagePanel* editPanel, ee::PropertySettingPanel* propertyPanel);

	protected:
		virtual wxSizer* InitLayout();

	}; // BodyEditCmpt
}

#endif // EMODELING_BODY_EDIT_CMPT_H
