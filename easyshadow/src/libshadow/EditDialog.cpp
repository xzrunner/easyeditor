#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <ee/panel_msg.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>
#include <ee/Camera.h>

#include <wx/splitter.h>

namespace eshadow
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   Sprite* edited, const ee::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Shadow", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_stage(NULL)
{
	assert(edited);

	Symbol* symbol = const_cast<Symbol*>(&edited->GetSymbol());
	symbol->Retain();
	m_symbol = symbol;
	m_symbol->ReloadTexture();
	SetTitle(symbol->GetFilepath());

	InitLayout(glctx, edited, sprite_impl);

	ee::SetWndDirtySJ::Instance()->SetDirty();
}

EditDialog::~EditDialog()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void EditDialog::InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
							const ee::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* split = new wxSplitterWindow(this);

	StagePanel* stage = new StagePanel(split, this, glctx, edited, sprite_impl);
	m_stage = stage;

	InitCamera(m_stage->GetCamera(), edited);

	ToolbarPanel* toolbar = new ToolbarPanel(split, stage);

	split->SetSashGravity(0.85f);
	split->SplitVertically(stage, toolbar);
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
//			FileSaver::Store(filepath, m_symbol);
		m_symbol->RefreshThumbnail(filepath);
		ee::SpriteFactory::Instance()->UpdateBoundings(*m_symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		m_symbol->LoadFromFile(m_symbol->GetFilepath());
		Destroy();
	}
}

void EditDialog::InitCamera(ee::Camera* cam, ee::Sprite* spr) const
{
	ee::Rect r = spr->GetRect();

	wxSize sz = GetSize();
	float scale = std::min(sz.GetWidth() / r.Width(), sz.GetHeight() / r.Height());
	cam->SetScale(1 / scale);

	cam->SetPosition(ee::Vector(0, 0));
}

}