#include "EditDialog.h"
#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "FileStorer.h"
#include "LibraryPanel.h"
#include "Symbol.h"
#include "ViewlistListImpl.h"

#include <ee/ViewlistPanel.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>

#include <wx/splitter.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol,
					   wxGLContext* glctx)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(symbol)
{
	SetTitle(symbol->GetFilepath());
	InitLayout(glctx);
	LoadSymbolInfo();

	m_property->SetDefaultPropertySetting();
}

void EditDialog::InitLayout(wxGLContext* glctx)
{
	wxSplitterWindow* right_split = new wxSplitterWindow(this);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split, glctx);
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

	m_property = new PropertySettingPanel(split);

	split->SetSashGravity(0.65f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* EditDialog::InitLayoutCenter(wxWindow* parent, wxGLContext* glctx)
{
	m_stage = new StagePanel(parent, this, m_symbol, m_property, 
		static_cast<ecomplex::LibraryPanel*>(m_library), glctx);
	m_property->SetEditPanel(m_stage->GetStageImpl());
	return m_stage;
}

wxWindow* EditDialog::InitLayoutRight(wxWindow* parent)
{
	m_viewlist = new ee::ViewlistPanel(parent);
	m_viewlist->SetListImpl(new ViewlistListImpl(m_stage, m_stage->GetStageImpl(), m_stage));
	return m_viewlist;
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES) 
	{
		const std::string& filepath = m_symbol->GetFilepath();
		if (filepath != "group") {
			FileStorer::Store(filepath.c_str(), m_symbol);
			m_symbol->RefreshThumbnail(filepath, true);
		}
		m_symbol->InitBounding();
		ee::SpriteFactory::Instance()->UpdateBoundings(*m_symbol);
		Destroy();
	} 
	else if (val == wxID_NO) 
	{
		m_symbol->LoadFromFile(m_symbol->GetFilepath());
		Destroy();
	}
}

void EditDialog::LoadSymbolInfo()
{
	for (size_t i = 0, n = m_symbol->m_sprites.size(); i < n; ++i) {
		m_library->AddSymbol(const_cast<ee::Symbol*>(&m_symbol->m_sprites[i]->GetSymbol()));
		m_viewlist->Insert(m_symbol->m_sprites[i]);
	}
}

}