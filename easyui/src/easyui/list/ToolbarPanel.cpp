#include "ToolBarPanel.h"
#include "UIList.h"

namespace eui
{
namespace list
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, UIList* list, d2d::EditPanel* editpanel)
	: d2d::ToolbarPanel(parent, NULL)
	, m_list(list)
	, m_editpanel(editpanel)
{
	SetSizer(initLayout());	
}

void ToolbarPanel::EnableFillingBtn(bool enable)
{
	m_btn_filling->Enable(enable);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->AddSpacer(10);
	{
		m_btn_filling = new wxButton(this, wxID_ANY, "filling");
		Connect(m_btn_filling->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnItemFilling));
		sizer->Add(m_btn_filling);
	}

	return sizer;
}

void ToolbarPanel::OnItemFilling(wxCommandEvent& event)
{
	bool succ = m_list->Filling();
	if (succ) {
		m_editpanel->SetCanvasDirty();
	}
}

}
}