#include "BodyEditCmpt.h"
#include "StagePanel.h"
#include "SelectBodyOP.h"

#include <ee/ArrangeSpriteConfig.h>
#include <ee/ArrangeSpriteOP.h>

namespace emodeling
{

BodyEditCmpt::BodyEditCmpt(wxWindow* parent, const std::string& name, 
						   StagePanel* editPanel, ee::PropertySettingPanel* property)
	: ee::EditCMPT(parent, name, editPanel->GetStageImpl())
{
	ee::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_editop = new ee::ArrangeSpriteOP<SelectBodyOP>(editPanel, editPanel->GetStageImpl(), editPanel, property, NULL, cfg);
}

wxSizer* BodyEditCmpt::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	return topSizer;
}

}