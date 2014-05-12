
#include "BodyEditCmpt.h"
#include "StagePanel.h"
#include "SelectBodyOP.h"

using namespace emodeling;

BodyEditCmpt::BodyEditCmpt(wxWindow* parent, const wxString& name, 
						   StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectBodyOP>(editPanel, editPanel, propertyPanel, NULL, false);
}

wxSizer* BodyEditCmpt::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	return topSizer;
}
