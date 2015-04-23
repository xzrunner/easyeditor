#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileSaver.h"

#include <wx/splitter.h>

namespace etexture
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnClose)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Sprite* edited, 
					   const std::vector<d2d::ISprite*>& bg_sprites)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_stage(NULL)
{
	assert(edited);

	Symbol* symbol = const_cast<Symbol*>(&edited->getSymbol());
	symbol->Retain();
	m_symbol = symbol;
	m_symbol->reloadTexture();
	SetTitle(symbol->getFilepath());

	InitLayout(edited, bg_sprites);

	m_stage->setTitleStatus(true);
}

EditDialog::~EditDialog()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

// void EditDialog::InitLayout()
// {
// 	wxSplitterWindow* vert_splitter = new wxSplitterWindow(this);
// //	wxSplitterWindow* hori_splitter = new wxSplitterWindow(vert_splitter);
// 
// 	LibraryPanel* library = new LibraryPanel(vert_splitter);
// 	StagePanel* stage = new StagePanel(vert_splitter, this, m_symbol, library);
// 	m_stage = stage;
// 	vert_splitter->SetSashGravity(0.15f);
// 	vert_splitter->SplitVertically(library, stage);
// }

void EditDialog::InitLayout(d2d::ISprite* edited, const std::vector<d2d::ISprite*>& bg_sprites)
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(this);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	LibraryPanel* library = new LibraryPanel(left_splitter);
	StagePanel* stage = new StagePanel(left_splitter, this, edited, bg_sprites, library);
	m_stage = stage;

	left_splitter->SetSashGravity(0.15f);
	left_splitter->SplitVertically(library, stage);

	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, stage, NULL);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, toolbar);
}

void EditDialog::OnClose(wxCloseEvent& event)
{
	if (m_stage->isDirty())
	{
		d2d::ExitDlg dlg(this);
		int val = dlg.ShowModal();
		if (val == wxID_OK)
		{
			const wxString& filepath = m_symbol->getFilepath();
			FileSaver::Store(filepath, m_symbol);
			m_symbol->RefreshThumbnail(filepath);
			d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
		}
		else if (val == wxID_CANCEL)
		{
			m_symbol->loadFromFile(m_symbol->getFilepath());
		}
	}

	if(IsModal()) {
		EndModal(wxID_CANCEL);
	} else {
		SetReturnCode(wxID_CANCEL);
		Hide();
	}

	if (!event.CanVeto()) {
		Destroy();
	}
}

}