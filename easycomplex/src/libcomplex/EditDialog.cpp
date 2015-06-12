#include "EditDialog.h"
#include "StagePanel.h"
#include "FileSaver.h"
#include "LibraryPanel.h"
#include "Symbol.h"
#include "SymbolPropertySetting.h"

#include <wx/splitter.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(symbol)
{
	SetTitle(symbol->getFilepath());
	InitLayout();
	LoadSymbolInfo();
}

void EditDialog::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(this);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.15f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);
}

wxWindow* EditDialog::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new ecomplex::LibraryPanel(split);

	m_property = new d2d::PropertySettingPanel(split);

	split->SetSashGravity(0.65f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* EditDialog::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, this, m_symbol, m_property, 
		static_cast<ecomplex::LibraryPanel*>(m_library));
	m_property->SetPropertySetting(new ecomplex::SymbolPropertySetting(m_stage, m_symbol));
	return m_stage;
}

wxWindow* EditDialog::InitLayoutRight(wxWindow* parent)
{
	m_viewlist = new d2d::ViewlistPanel(parent, m_stage, m_stage, m_property);
	return m_viewlist;
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	OnClose(!event.CanVeto(), wxID_CANCEL);
}

void EditDialog::OnClose(bool force, int returncode)
{
	if (m_stage->isDirty())
	{
	 	d2d::ExitDlg dlg(this);
	 	int val = dlg.ShowModal();
	 	if (val == wxID_OK)
	 	{
	 		const wxString& filepath = m_symbol->getFilepath();
	 		FileSaver::store(filepath, m_symbol);
	 		m_symbol->RefreshThumbnail(filepath);
	 		d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
	 	}
	 	else if (val == wxID_CANCEL)
	 	{
	 		m_symbol->loadFromFile(m_symbol->getFilepath());
	 	}
	}

	if(IsModal())
	{
		EndModal(returncode);
	}
	else
	{
		SetReturnCode(returncode);
		Hide();
	}

	if(force)
	{
		Destroy();
	}
}

void EditDialog::LoadSymbolInfo()
{
	for (size_t i = 0, n = m_symbol->m_sprites.size(); i < n; ++i) {
		m_library->AddSymbol(const_cast<d2d::ISymbol*>(&m_symbol->m_sprites[i]->getSymbol()));
		m_viewlist->Insert(m_symbol->m_sprites[i]);
	}
}

}