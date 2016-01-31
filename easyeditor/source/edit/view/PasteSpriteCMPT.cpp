#include "PasteSpriteCMPT.h"
#include "PasteSpriteOP.h"

namespace ee
{

PasteSpriteCMPT::PasteSpriteCMPT(wxWindow* parent, const std::string& name, 
								 wxWindow* stage_wnd, EditPanelImpl* stage, 
								 MultiSpritesImpl* spritesImpl)
	: EditCMPT(parent, name, stage)
{
	m_editop = new PasteSpriteOP(stage_wnd, stage, spritesImpl, this);
}

wxSizer* PasteSpriteCMPT::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("tools"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	sizer->Add(m_xMirror = new wxCheckBox(this, wxID_ANY, wxT("Horizontal Mirror")));
	sizer->AddSpacer(10);
	sizer->Add(m_yMirror = new wxCheckBox(this, wxID_ANY, wxT("Vertical Mirror")));

	return sizer;
}

}