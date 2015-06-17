
#include "BodyEditCmpt.h"
#include "StagePanel.h"
#include "SelectBodyOP.h"

using namespace emodeling;

BodyEditCmpt::BodyEditCmpt(wxWindow* parent, const wxString& name, 
						   StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel)
{
	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_editOP = new d2d::ArrangeSpriteOP<SelectBodyOP>(editPanel, editPanel, propertyPanel, NULL, NULL, cfg);
}

wxSizer* BodyEditCmpt::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	return topSizer;
}
