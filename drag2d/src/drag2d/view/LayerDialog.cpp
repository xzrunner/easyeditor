#include "LayerDialog.h"

#include "dataset/Layer.h"

namespace d2d
{

LayerDialog::LayerDialog(wxWindow* parent, Layer* layer, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "Layer", pos)
	, m_layer(layer)
{
	InitLayout();
}

void LayerDialog::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		m_check_view = new wxCheckBox(this, wxID_ANY, "¿ÉÊÓ");
		m_check_view->SetValue(m_layer->visible);
		Connect(m_check_view->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LayerDialog::OnChangeVisible));
		sizer->Add(m_check_view);
	}
	{
		m_check_edit = new wxCheckBox(this, wxID_ANY, "¿É±à¼­");
		m_check_edit->SetValue(m_layer->editable);
		Connect(m_check_edit->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LayerDialog::OnChangeEditable));
		sizer->Add(m_check_edit);
	}
	sizer->Fit(this);
	sizer->Layout();
	SetSizer(sizer);
}

void LayerDialog::OnChangeVisible(wxCommandEvent& event)
{
	m_layer->visible = event.IsChecked();
}

void LayerDialog::OnChangeEditable(wxCommandEvent& event)
{
	m_layer->editable = event.IsChecked();
}

}
