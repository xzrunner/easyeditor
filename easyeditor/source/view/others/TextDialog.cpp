#include "TextDialog.h"
#include "FontBlankSprite.h"

namespace ee
{

BEGIN_EVENT_TABLE(TextDialog, wxDialog)
	EVT_CLOSE(TextDialog::OnClose)
END_EVENT_TABLE()

TextDialog::TextDialog(wxWindow* parent, const std::shared_ptr<FontBlankSprite>& font)
	: wxDialog(parent, wxID_ANY, wxT("Text"), wxDefaultPosition, wxSize(600, 400), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_font(font)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_text_ctrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE); 
	m_text_ctrl->SetEditable(true);
	m_text_ctrl->SetValue(m_font->GetTextContext());

	sizer->Add(m_text_ctrl, 1, wxEXPAND);
	SetSizer(sizer);
}

void TextDialog::OnClose(wxCloseEvent& event)
{
	std::string text;
	for (int i = 0, n = m_text_ctrl->GetNumberOfLines(); i < n; ++i) {
		text += m_text_ctrl->GetLineText(i) + '\n';
	}

	m_font->SetTextContent(text);

	Destroy();
}

}