#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <wx/splitter.h>

namespace eshadow
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnClose)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Sprite* edited, 
					   const std::vector<d2d::ISprite*>& bg_sprites)
	: wxDialog(parent, wxID_ANY, "Edit Shadow", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_stage(NULL)
{
	assert(edited);

	Symbol* symbol = const_cast<Symbol*>(&edited->getSymbol());
	symbol->Retain();
	m_symbol = symbol;
	m_symbol->ReloadTexture();
	SetTitle(symbol->GetFilepath());

	InitLayout(edited, bg_sprites);

	m_stage->setTitleStatus(true);
}

EditDialog::~EditDialog()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void EditDialog::InitLayout(d2d::ISprite* edited, const std::vector<d2d::ISprite*>& bg_sprites)
{
	wxSplitterWindow* split = new wxSplitterWindow(this);

	StagePanel* stage = new StagePanel(split, this, edited, bg_sprites);
	m_stage = stage;

	ToolbarPanel* toolbar = new ToolbarPanel(split, stage);

	split->SetSashGravity(0.85f);
	split->SplitVertically(stage, toolbar);
}

void EditDialog::OnClose(wxCloseEvent& event)
{
	if (m_stage->isDirty())
	{
		d2d::ExitDlg dlg(this);
		int val = dlg.ShowModal();
		if (val == wxID_OK)
		{
			const std::string& filepath = m_symbol->GetFilepath();
//			FileSaver::Store(filepath, m_symbol);
			m_symbol->RefreshThumbnail(filepath);
			d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
		}
		else if (val == wxID_CANCEL)
		{
			m_symbol->LoadFromFile(m_symbol->GetFilepath());
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