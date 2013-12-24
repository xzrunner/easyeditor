
#include "FixtureEditCmpt.h"
#include "StagePanel.h"
#include "SelectFixtureOP.h"

using namespace emodeling;

FixtureEditCmpt::FixtureEditCmpt(wxWindow* parent, const wxString& name, 
								 StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new SelectFixtureOP(editPanel, propertyPanel);
}

wxSizer* FixtureEditCmpt::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	return topSizer;
}
