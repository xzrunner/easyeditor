#include "EditDialog.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "InputPanel.h"
#include "ToolBarPanel.h"

#include <wx/splitter.h>

namespace etext
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   Sprite* spr, const d2d::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Text", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
{
	InitLayout(glctx, spr, sprite_impl);
}

void EditDialog::InitLayout(wxGLContext* glctx, Sprite* spr, 
							const d2d::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* right_split = new wxSplitterWindow(this);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split, glctx, spr, sprite_impl);
	wxWindow* right = InitLayoutRight(right_split, spr);

	left_split->SetSashGravity(0.15f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.85f);
	right_split->SplitVertically(left_split, right);
}

wxWindow* EditDialog::InitLayoutLeft(wxWindow* parent)
{
	m_library = new LibraryPanel(parent);
	return m_library;
}

wxWindow* EditDialog::InitLayoutCenter(wxWindow* parent, wxGLContext* glctx,
									   Sprite* spr, const d2d::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_stage = new StagePanel(split, this, glctx, spr, sprite_impl, m_library);

	m_input = new InputPanel(split, spr, m_stage->GetStageImpl());

	split->SetSashGravity(0.6f);
	split->SplitHorizontally(m_stage, m_input);

	return split;
}

wxWindow* EditDialog::InitLayoutRight(wxWindow* parent, Sprite* spr)
{
	m_toolbar = new ToolbarPanel(parent, m_stage, spr);
	return m_toolbar;
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

// 	d2d::ConfirmDialog dlg(this);
// 	int val = dlg.ShowModal();
// 	if (val == wxID_YES) 
// 	{
// 		m_symbol->InitBounding();
// 		const std::string& filepath = m_symbol->GetFilepath();
// 		FileStorer::Store(filepath.c_str(), m_symbol);
// 		m_symbol->RefreshThumbnail(filepath, true);
// 		d2d::SpriteFactory::Instance()->UpdateBoundings(*m_symbol);
// 		Destroy();
// 	} 
// 	else if (val == wxID_NO) 
// 	{
// 		m_symbol->LoadFromFile(m_symbol->GetFilepath());
// 		Destroy();
// 	}
}

}