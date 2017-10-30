#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "Mesh.h"

#include <ee/ConfirmDialog.h>
#include <ee/SpritePool.h>

#include <wx/splitter.h>

namespace emesh
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx, 
					   const std::shared_ptr<Sprite>& edited, const ee::MultiSpritesImpl* sprite_impl)
	: wxDialog(parent, wxID_ANY, "Edit Mesh", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_spr(edited)
{
	auto sym = std::dynamic_pointer_cast<Symbol>(m_spr->GetSymbol());
	SetTitle(sym->GetFilepath());
	InitLayout(glctx, edited, sprite_impl);

	sym->SetPause(true);
}

EditDialog::~EditDialog()
{
	auto sym = std::dynamic_pointer_cast<Symbol>(m_spr->GetSymbol());
	sym->SetPause(false);
}

void EditDialog::InitLayout(wxGLContext* glctx, const ee::SprPtr& edited,
							const ee::MultiSpritesImpl* sprite_impl)
{
 	wxSplitterWindow* splitter = new wxSplitterWindow(this);
 
 	StagePanel* stage = new StagePanel(splitter, this, glctx, edited, sprite_impl);
	auto sym = std::dynamic_pointer_cast<Symbol>(m_spr->GetSymbol());
	stage->SetMeshSymbol(sym);
 	m_stage = stage;
 	ee::ToolbarPanel* toolbar = new ToolbarPanel(splitter, stage, false, m_spr);
 
 	splitter->SetSashGravity(0.85f);
 	splitter->SplitVertically(stage, toolbar);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
//	m_spr->UpdateBounding();

	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	auto sym = std::dynamic_pointer_cast<Symbol>(m_spr->GetSymbol());

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		sym->GetMesh()->StoreToTransforom(m_spr->GetMeshTrans());

// 		const std::string& filepath = sym.GetFilepath();
// 		FileIO::Store(sym.GetFilepath().c_str(), &sym);
// 		sym.RefreshThumbnail(filepath);

		ee::SpritePool::Instance()->UpdateBoundings(*sym);
		Destroy();
	}
	else if (val == wxID_NO)
	{
//		sym.LoadFromFile(sym.GetFilepath());
		Destroy();
	}
}

}