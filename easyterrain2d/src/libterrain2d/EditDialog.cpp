#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"

#include <wx/splitter.h>

namespace eterrain2d
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx, 
					   Sprite* edited, const d2d::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Terrain2D", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
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

	d2d::SetWndDirtySJ::Instance()->SetDirty();
}

EditDialog::~EditDialog()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void EditDialog::InitLayout(wxGLContext* glctx, d2d::ISprite* edited, 
							const d2d::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(this);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	LibraryPanel* library = new LibraryPanel(left_splitter);
	StagePanel* stage = new StagePanel(left_splitter, this, glctx, edited, sprite_impl, library);
	m_stage = stage;

	InitCamera(m_stage->GetCamera(), edited);

	left_splitter->SetSashGravity(0.15f);
	left_splitter->SplitVertically(library, stage);

	ToolbarPanel* toolbar = new ToolbarPanel(right_splitter, stage);

	right_splitter->SetSashGravity(0.85f);
	right_splitter->SplitVertically(left_splitter, toolbar);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	d2d::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		const std::string& filepath = m_symbol->GetFilepath();
		FileIO::StoreSymbol(filepath.c_str(), m_symbol);
		m_symbol->RefreshThumbnail(filepath);
		d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		m_symbol->LoadFromFile(m_symbol->GetFilepath());
		Destroy();
	}
}

void EditDialog::InitCamera(d2d::Camera* cam, d2d::ISprite* spr) const
{
	d2d::Rect r = spr->GetRect();

	wxSize sz = GetSize();
	float scale = std::min(sz.GetWidth() / r.xLength(), sz.GetHeight() / r.yLength());
	cam->SetScale(1 / scale);

	cam->SetPosition(d2d::Vector(0, 0));
}

}