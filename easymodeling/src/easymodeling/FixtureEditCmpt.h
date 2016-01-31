
#ifndef EMODELING_FIXTURE_EDIT_CMPT_H
#define EMODELING_FIXTURE_EDIT_CMPT_H



namespace emodeling
{
	class StagePanel;

	class FixtureEditCmpt : public ee::EditCMPT
	{
	public:
		FixtureEditCmpt(wxWindow* parent, const wxString& name,
			StagePanel* editPanel, ee::PropertySettingPanel* propertyPanel);

	protected:
		virtual wxSizer* InitLayout();

	}; // FixtureEditCmpt
}

#endif // EMODELING_FIXTURE_EDIT_CMPT_H
