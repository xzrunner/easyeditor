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
#include <ee/SymbolType.h>
#include <ee/FileHelper.h>

#include <sprite2/S2_Sprite.h>

#include <wx/splitter.h>

namespace ecomplex
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* sym,
					   wxGLContext* glctx, ee::CrossGuides* guides)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sym(sym)
{
	SetTitle(sym->GetFilepath());
	InitLayout(glctx, guides);
	LoadSymbolInfo();

	m_property->SetDefaultPropertySetting();
}

void EditDialog::InitLayout(wxGLContext* glctx, ee::CrossGuides* guides)
{
	wxSplitterWindow* right_split = new wxSplitterWindow(this);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split, glctx, guides);
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

wxWindow* EditDialog::InitLayoutCenter(wxWindow* parent, wxGLContext* glctx,
									   ee::CrossGuides* guides)
{
	m_stage = new StagePanel(parent, this, m_sym, m_property, 
		static_cast<ecomplex::LibraryPanel*>(m_library), glctx, guides);
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
		m_sym->SetEditDirty(true);
		const std::string& filepath = m_sym->GetFilepath();
		if (filepath != ee::SYM_GROUP_TAG) {
			FileStorer::Store(filepath, m_sym, ee::FileHelper::GetFileDir(filepath));
			m_sym->RefreshThumbnail(filepath, true);
		}
// 		m_sym->InitBounding();
// 		ee::SpriteFactory::Instance()->UpdateBoundings(*m_sym);
		Destroy();
	} 
	else if (val == wxID_NO) 
	{
		m_sym->LoadFromFile(m_sym->GetFilepath());
		Destroy();
	}
}

void EditDialog::LoadSymbolInfo()
{
	const std::vector<s2::Sprite*>& children = m_sym->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		m_library->AddSymbol(dynamic_cast<ee::Symbol*>(child->GetSymbol()));
		m_viewlist->Insert(child);
	}	
}

}