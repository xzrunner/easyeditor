
#include "FixtureEditCmpt.h"
#include "StagePanel.h"
#include "SelectFixtureOP.h"

using namespace emodeling;

FixtureEditCmpt::FixtureEditCmpt(wxWindow* parent, const wxString& name, 
								 StagePanel* editPanel, ee::PropertySettingPanel* propertyPanel)
	: ee::EditCMPT(parent, name, editPanel->GetStageImpl())
{
	m_editOP = new SelectFixtureOP(editPanel, propertyPanel);
}

wxSizer* FixtureEditCmpt::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	return topSizer;
}
