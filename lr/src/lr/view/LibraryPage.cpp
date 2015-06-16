#include "LibraryPage.h"
#include "StagePanel.h"

#include "dataset/Layer.h"

namespace lr
{

LibraryPage::LibraryPage(wxWindow* parent, const std::string& name,
						 d2d::AbstractEditOP* editop)
	: d2d::ILibraryPage(parent, name)
	, m_layer(new Layer)
	, m_editop(editop)
{
	m_editop->Retain();
	InitLayout();
}

LibraryPage::~LibraryPage()
{
	m_editop->Release();
	m_layer->Release();
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
}

void LibraryPage::OnActive()
{
	m_editop->OnActive();
}

void LibraryPage::UpdateStatusFromLayer()
{
	m_visible_ctrl->SetValue(m_layer->IsVisible());	
	m_editable_ctrl->SetValue(m_layer->IsEditable());
}

void LibraryPage::SetLayer(Layer* layer)
{
	if (m_layer != layer) {
		m_layer->Release();
		layer->Retain();
		m_layer = layer;
	}
}

void LibraryPage::InitLayoutExtend(wxSizer* sizer)
{
	{
		m_visible_ctrl = new wxCheckBox(this, wxID_ANY, wxT("可见"));
		m_visible_ctrl->SetValue(m_layer->IsVisible());
		Connect(m_visible_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnChangeVisible));
		sizer->Add(m_visible_ctrl, 0, wxALIGN_LEFT);
	}
	{
		m_editable_ctrl = new wxCheckBox(this, wxID_ANY, wxT("编辑"));
		m_editable_ctrl->SetValue(m_layer->IsEditable());
		Connect(m_editable_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnChangeEditable));
		sizer->Add(m_editable_ctrl, 0, wxALIGN_LEFT);
	}
	sizer->AddSpacer(20);
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("导入symbol"), wxEmptyString, 
		wxEmptyString, "*.*", wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filepath = filenames[i];
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->insert(symbol);
			symbol->Release();
		}

		if (!filenames.empty() && m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

void LibraryPage::OnChangeVisible(wxCommandEvent& event)
{
	m_layer->SetVisible(event.IsChecked());
	m_canvas->Refresh();
}

void LibraryPage::OnChangeEditable(wxCommandEvent& event)
{
	m_layer->SetEditable(event.IsChecked());
}

}