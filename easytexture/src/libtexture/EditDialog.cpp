#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileSaver.h"

#include <ee/panel_msg.h>
#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>
#include <ee/OrthoCamera.h>
#include <ee/CameraMgr.h>

#include <wx/splitter.h>

namespace etexture
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   Sprite* edited, const ee::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Texture", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_stage(NULL)
{
	assert(edited);

	Symbol* symbol = const_cast<Symbol*>(&edited->GetSymbol());
	symbol->AddReference();
	m_symbol = symbol;
//	m_symbol->ReloadTexture();
	SetTitle(symbol->GetFilepath());

	InitLayout(glctx, edited, sprite_impl);

	ee::SetWndDirtySJ::Instance()->SetDirty();

	m_visible_tex_edge = ee::Config::Instance()->GetSettings().visible_tex_edge;
	ee::Config::Instance()->GetSettings().visible_tex_edge = true;
}

EditDialog::~EditDialog()
{
	ee::Config::Instance()->GetSettings().visible_tex_edge = m_visible_tex_edge;

	if (m_symbol) {
		m_symbol->RemoveReference();
	}
}

void EditDialog::InitLayout(wxGLContext* glctx, ee::Sprite* edited, const ee::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(this);
	wxSplitterWindow* left_splitter = new wxSplitterWindow(right_splitter);

	LibraryPanel* library = new LibraryPanel(left_splitter);
	StagePanel* stage = new StagePanel(left_splitter, this, glctx, edited, sprite_impl, library);
	m_stage = stage;

	InitCamera(edited);

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

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		const std::string& filepath = m_symbol->GetFilepath();
		FileSaver::Store(filepath.c_str(), m_symbol);
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

void EditDialog::InitCamera(ee::Sprite* spr) const
{
	if (!ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		return;
	}

	wxSize sz = GetSize();
	sm::vec2 r_sz = spr->GetRect().Size();
	float scale = std::min(sz.GetWidth() / r_sz.x, sz.GetHeight() / r_sz.y);

	ee::OrthoCamera* cam = static_cast<ee::OrthoCamera*>(ee::CameraMgr::Instance()->GetCamera());
	cam->SetScale(1 / scale);
	cam->SetPosition(sm::vec2(0, 0));
}

}