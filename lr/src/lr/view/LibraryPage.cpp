#include "LibraryPage.h"
#include "LibraryPanel.h"

#include "dataset/Layer.h"
#include "dataset/LayerFactory.h"

namespace lr
{

LibraryPage::LibraryPage(LibraryPanel* library, const std::string& name, 
						 LayerType type, int id)
	: d2d::ILibraryPage(library->GetNotebook(), name)
	, m_layer_type(type)
	, m_curr_op_idx(-1)
{
	m_layer = LayerFactory::Create(id, library, type);
	InitLayout();
}

LibraryPage::~LibraryPage()
{
	m_layer->Release();

	for_each(m_editops.begin(), m_editops.end(), d2d::ReleaseObjectFunctor<d2d::AbstractEditOP>());
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
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

void LibraryPage::AddEditOP(d2d::AbstractEditOP* editop)
{
	editop->Retain();
	m_editops.push_back(editop);
	if (m_curr_op_idx < 0) {
		m_curr_op_idx = 0;
	}
}

d2d::AbstractEditOP* LibraryPage::GetNextEditOP()
{
	if (m_editops.empty()) {
		return NULL;
	}

	++m_curr_op_idx;
	if (m_curr_op_idx >= m_editops.size()) {
		m_curr_op_idx = 0;
	}
	return m_editops[m_curr_op_idx];
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
			std::string filepath = filenames[i].ToStdString();
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->Insert(symbol);
			symbol->Release();
		}

		if (!filenames.empty() && m_canvas) {
			m_canvas->ResetViewport();
		}
	}
}

void LibraryPage::OnChangeVisible(wxCommandEvent& event)
{
	m_layer->SetVisible(event.IsChecked());
	m_canvas->SetDirty();
}

void LibraryPage::OnChangeEditable(wxCommandEvent& event)
{
	m_layer->SetEditable(event.IsChecked());
}

}