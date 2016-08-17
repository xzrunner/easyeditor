#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <ee/panel_msg.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>
#include <ee/OrthoCamera.h>
#include <ee/CameraMgr.h>

#include <sprite2/BoundingBox.h>

#include <wx/splitter.h>

namespace eshadow
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx,
					   Sprite* edited, const ee::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Shadow", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sym(NULL)
	, m_stage(NULL)
{
	assert(edited);

	Symbol* sym = dynamic_cast<Symbol*>(edited->GetSymbol());
	sym->AddReference();
	m_sym = sym;
	m_sym->ReloadTexture();
	SetTitle(sym->GetFilepath());

	InitLayout(glctx, edited, sprite_impl);

	ee::SetWndDirtySJ::Instance()->SetDirty();
}

EditDialog::~EditDialog()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void EditDialog::InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
							const ee::MultiSpritesImpl* sprite_impl)
{
	wxSplitterWindow* split = new wxSplitterWindow(this);

	StagePanel* stage = new StagePanel(split, this, glctx, edited, sprite_impl);
	m_stage = stage;

	InitCamera(edited);

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
		const std::string& filepath = m_sym->GetFilepath();
//			FileSaver::Store(filepath, m_sym);
		m_sym->RefreshThumbnail(filepath);
		ee::SpriteFactory::Instance()->UpdateBoundings(*m_sym);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		m_sym->LoadFromFile(m_sym->GetFilepath());
		Destroy();
	}
}

void EditDialog::InitCamera(ee::Sprite* spr) const
{
	if (!ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		return;
	}

	wxSize sz = GetSize();
	sm::vec2 r_sz = spr->GetBounding()->GetSize().Size();
	float scale = std::min(sz.GetWidth() / r_sz.x, sz.GetHeight() / r_sz.y);

	ee::OrthoCamera* cam = static_cast<ee::OrthoCamera*>(ee::CameraMgr::Instance()->GetCamera());
	cam->SetScale(1 / scale);
	cam->SetPosition(sm::vec2(0, 0));
}

}