#include "OpenSymbolDialog.h"

#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/EditPanelImpl.h>

#include <easymesh.h>

#include <wx/wx.h>

namespace eanim
{

OpenSymbolDialog::OpenSymbolDialog(wxWindow* wnd, 
								   ee::EditPanelImpl* stage, 
								   ee::MultiSpritesImpl* sprites_impl)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
{	
}

void OpenSymbolDialog::Open(ee::Sprite* spr)
{
	if (!spr) {
		return;
	}

	if (spr->GetSymbol().GetFilepath().find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	m_sprites_impl->EnableObserve(false);
	m_stage->EnableObserve(false);
	m_stage->GetCanvas()->EnableObserve(false);
	m_stage->GetCanvas()->SetDrawable(false);

	if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(spr))
	{
		emesh::EditDialog dlg(m_wnd, sprite, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}

	m_sprites_impl->EnableObserve(true);
	m_stage->EnableObserve(true);
	m_stage->GetCanvas()->EnableObserve(true);
	m_stage->GetCanvas()->SetDrawable(true);
}

}