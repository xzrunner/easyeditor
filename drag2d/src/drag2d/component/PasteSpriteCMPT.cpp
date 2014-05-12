#include "PasteSpriteCMPT.h"

#include "operator/PasteSpriteOP.h"

namespace d2d
{

PasteSpriteCMPT::PasteSpriteCMPT(wxWindow* parent, const wxString& name, 
								 EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
								 PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new PasteSpriteOP(editPanel, spritesImpl, propertyPanel, this);
}

wxSizer* PasteSpriteCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("tools"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	sizer->Add(m_xMirror = new wxCheckBox(this, wxID_ANY, wxT("Horizontal Mirror")));
	sizer->AddSpacer(10);
	sizer->Add(m_yMirror = new wxCheckBox(this, wxID_ANY, wxT("Vertical Mirror")));

	return sizer;
}

} // d2d