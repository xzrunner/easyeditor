#include "TextDialog.h"

#include "dataset/FontSprite.h"

namespace d2d
{

TextDialog::TextDialog(wxWindow* parent, FontSprite* font)
	: wxDialog(parent, wxID_ANY, wxT("Text"), wxDefaultPosition, wxSize(600, 400), wxCLOSE_BOX | wxCAPTION)
	, m_font(font)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_text_ctrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE); 
	m_text_ctrl->SetEditable(true);
	m_text_ctrl->SetValue(m_font->GetTextContext());

	sizer->Add(m_text_ctrl, 1, wxEXPAND);
	SetSizer(sizer);
}

TextDialog::~TextDialog()
{
	wxString text;
	for (int i = 0, n = m_text_ctrl->GetNumberOfLines(); i < n; ++i) {
		text += m_text_ctrl->GetLineText(i) + '\n';
	}
	m_font->SetTextContent(text.ToStdString());
}

}