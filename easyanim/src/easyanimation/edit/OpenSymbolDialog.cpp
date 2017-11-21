#include "OpenSymbolDialog.h"

#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/CurrSprTreePath.h>

#include <easycomplex.h>
#include <easymesh.h>
#include <easyskeleton.h>
#include <easyaudio.h>

#include <sprite2/SymType.h>
#include <gum/Audio.h>

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

void OpenSymbolDialog::Open(const ee::SprPtr& spr)
{
	if (!spr) {
		return;
	}

	std::string filepath = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();
	if (filepath.find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	bool stop_audio = true;
	gum::Audio::Instance()->Stop();

	ee::CurrSprTreePath::Instance()->Push(spr);

	m_sprites_impl->EnableObserve(false);
	m_stage->EnableObserve(false);
	m_stage->SetActive(false);
	m_stage->GetCanvas()->EnableObserve(false);
	m_stage->GetCanvas()->SetDrawable(false);

	ee::SymPtr edited_sym = nullptr;
	int type = spr->GetSymbol()->Type();
	if (type == s2::SYM_COMPLEX)
	{
		edited_sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
		auto complex = std::dynamic_pointer_cast<ecomplex::Symbol>(spr->GetSymbol());
		ecomplex::EditDialog dlg(m_wnd, complex, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	else if (type == s2::SYM_MESH)
	{
		edited_sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
		auto mesh = std::dynamic_pointer_cast<emesh::Sprite>(spr);
		emesh::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), mesh, m_sprites_impl);
		dlg.ShowModal();
	} 
	else if (type == s2::SYM_SKELETON) 
	{
		edited_sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
		auto sk = std::dynamic_pointer_cast<libskeleton::Sprite>(spr);
		libskeleton::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), sk);
		dlg.ShowModal();
	}
	else if (type == s2::SYM_AUDIO)
	{
		stop_audio = false;
		edited_sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());
		auto audio = std::dynamic_pointer_cast<eaudio::Sprite>(spr);
		audio->Play();
	}

	m_sprites_impl->EnableObserve(true);
	m_stage->EnableObserve(true);
	m_stage->SetActive(true);
	m_stage->GetCanvas()->EnableObserve(true);
	m_stage->GetCanvas()->SetDrawable(true);

	if (edited_sym && edited_sym->IsEditDirty()) {
		ee::SetWndDirtySJ::Instance()->SetDirty();
		edited_sym->SetEditDirty(false);
	}

	ee::CurrSprTreePath::Instance()->Pop();

	if (stop_audio) {
		gum::Audio::Instance()->Stop();
	}
}

}