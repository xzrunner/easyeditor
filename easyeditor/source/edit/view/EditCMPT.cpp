#include "EditCMPT.h"
#include "EditOP.h"
#include "EditPanelImpl.h"
#include "std_functor.h"

#include <algorithm>

namespace ee
{

EditCMPT::EditCMPT(wxWindow* parent, const std::string& name, EditPanelImpl* stage, 
				   const std::string& child_name, bool vertical)
	: wxScrolledWindow(parent)
	, m_stage(stage)
	, m_editop(NULL)
	, m_name(name)
	, m_child_name(child_name)
	, m_child_sz(NULL)
	, m_vertical(vertical)
{
}

EditCMPT::~EditCMPT()
{
	if (m_editop)
		m_editop->RemoveReference();
 
 	for_each(m_children.begin(), m_children.end(), DeletePointerFunctor<EditCMPT>());
}

void EditCMPT::SetEditOP(EditOP* op)
{
	LoadEditOP(op);
}

wxSizer* EditCMPT::InitChildrenLayout()
{
	m_child_sz = m_vertical ? new wxBoxSizer(wxVERTICAL) : new wxBoxSizer(wxHORIZONTAL);

	if (m_children.size() > 1)
	{
		wxArrayString choices;
		for (size_t i = 0, n = m_children.size(); i < n; ++i)
			choices.Add(m_children[i]->m_name);

		long style = m_vertical ? wxRA_SPECIFY_COLS : wxRA_SPECIFY_ROWS;
		m_edit_choice = new wxRadioBox(this, wxID_ANY, m_child_name.empty() ? "Operation" : m_child_name, 
			wxDefaultPosition, wxDefaultSize, choices, 1, style);
		Connect(m_edit_choice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(EditCMPT::OnChangeEditType));
		m_child_sz->Add(m_edit_choice, 0);

		m_child_sz->AddSpacer(20);

		for (size_t i = 0, n = m_children.size(); i < n; ++i)
		{
			EditCMPT* child = m_children[i];
			child->SetSizer(child->InitLayout());
			m_child_sz->Add(child);

			m_child_sz->Show(i + CMPT_SIZER_INDEX_OFFSET, i == 0);
		}
	}
	else if (m_children.size() == 1)
	{
		EditCMPT* child = m_children[0];
		child->SetSizer(child->InitLayout());
		m_child_sz->Add(child);

		m_child_sz->Show((size_t)0, true);
	}

	if (!m_children.empty()) 
		m_children[0]->LoadEditOP(this);

	return m_child_sz;
}

void EditCMPT::SetChoice(size_t index)
{
	if (index >= 0 && index < m_children.size())
	{
		m_edit_choice->SetSelection(index);
		OnChangeEditType(wxCommandEvent());
	}
}

void EditCMPT::LoadEditOP()
{
	m_stage->SetEditOP(m_editop);
}

void EditCMPT::LoadEditOP(EditOP* op)
{
	if (m_editop == op) {
		return;
	}

	if (op) {
		op->AddReference();
	}
	if (m_editop) {
		m_editop->Clear();
		m_editop->RemoveReference();
	}
	m_editop = op;

	m_stage->SetEditOP(m_editop);
}

void EditCMPT::LoadEditOP(EditCMPT* cmpt)
{
	if (cmpt->m_editop == m_editop) {
		return;
	}

	if (m_editop) {
		m_editop->AddReference();
	}
	if (cmpt->m_editop)
	{
		cmpt->m_editop->Clear();
		cmpt->m_editop->RemoveReference();
	}
	cmpt->m_editop = m_editop;

	m_stage->SetEditOP(m_editop);
}

void EditCMPT::OnChangeEditType(wxCommandEvent& event)
{
	int index = m_edit_choice->GetSelection();

	for (size_t i = 0, n = m_children.size(); i < n; ++i)
	{
		m_child_sz->Show(i + CMPT_SIZER_INDEX_OFFSET, i == index);
		m_children[i]->Show(i == index);

		m_children[i]->Layout();
	}
	Layout();

	if (index < static_cast<int>(m_children.size())) {
		m_children[index]->LoadEditOP(this);
	}
}

}