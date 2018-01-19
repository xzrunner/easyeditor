#include "OpenSymbolDialog.h"
#include "EditDialog.h"
#include "Sprite.h"
#include "Symbol.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyscale9.h>
#include <easyanim.h>
#include <easytexture.h>
#include <easyicon.h>
#include <easyshadow.h>
#include <easytext.h>
#include <easyterrain2d.h>
#include <easyskeleton.h>
#include <easyaudio.h>

#include <ee/EditPanelImpl.h>
#include <ee/FontBlankSprite.h>
#include <ee/TextDialog.h>
#include <ee/ViewlistPanel.h>
#include <ee/panel_msg.h>
#include <ee/CurrSprTreePath.h>

#include <easynode3d/EditDialog.h>
#include <easynode3d/Sprite.h>

#include <gum/Audio.h>

namespace ecomplex
{

OpenSymbolDialog::OpenSymbolDialog(wxWindow* wnd, ee::EditPanelImpl* stage,
								   ee::MultiSpritesImpl* sprites_impl)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_viewlist(NULL)
{
}

void OpenSymbolDialog::Open(const ee::SprPtr& spr, ee::CrossGuides* guides)
{
	if (!spr) {
		return;
	}

	if (std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath().find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	bool stop_audio = true;
	gum::Audio::Instance()->Stop();

	ee::CurrSprTreePath::Instance()->Push(spr);

	auto canvas = m_stage->GetCanvas();

	m_sprites_impl->EnableObserve(false);
	m_stage->EnableObserve(false);
	m_stage->SetActive(false);
	canvas->EnableObserve(false);
	canvas->SetDrawable(false);
	if (m_viewlist) {
		m_viewlist->EnableObserve(false);
	}

	ee::SymPtr edited_sym = nullptr;
	if (auto complex = std::dynamic_pointer_cast<ecomplex::Sprite>(spr))
	{
		auto sym = std::dynamic_pointer_cast<ecomplex::Symbol>(complex->GetSymbol());
		edited_sym = sym;
 		EditDialog dlg(m_wnd, std::dynamic_pointer_cast<ecomplex::Symbol>(edited_sym), canvas->GetGLContext(), guides);
 		dlg.ShowModal();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (auto anim = std::dynamic_pointer_cast<libanim::Sprite>(spr))
	{
		auto sym = std::dynamic_pointer_cast<libanim::Symbol>(anim->GetSymbol());
		edited_sym = sym;
		libanim::PreviewDialog dlg(m_wnd, sym, canvas->GetGLContext());
 		dlg.ShowModal();
	}
	else if (auto patch9 = std::dynamic_pointer_cast<escale9::Sprite>(spr))
 	{
		auto sym = std::dynamic_pointer_cast<escale9::Symbol>(patch9->GetSymbol());
		edited_sym = sym;
  		escale9::EditDialog dlg(m_wnd, sym, canvas->GetGLContext());
  		dlg.ShowModal();
 	}
	else if (auto mesh = std::dynamic_pointer_cast<emesh::Sprite>(spr))
	{
		emesh::EditDialog dlg(m_wnd, canvas->GetGLContext(), mesh, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (auto font = std::dynamic_pointer_cast<ee::FontBlankSprite>(spr))
	{
		ee::TextDialog dlg(m_wnd, font);
		dlg.ShowModal();
	}
	else if (auto tex = std::dynamic_pointer_cast<etexture::Sprite>(spr))
	{
		etexture::EditDialog dlg(m_wnd, canvas->GetGLContext(), tex, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (auto icon = std::dynamic_pointer_cast<eicon::Sprite>(spr))
	{
		eicon::EditDialog dlg(m_wnd, canvas->GetGLContext(), spr, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (auto text = std::dynamic_pointer_cast<etext::Sprite>(spr))
	{
		etext::EditDialog dlg(m_wnd, canvas->GetGLContext(), text, m_sprites_impl);
		dlg.ShowModal();
	}
	//else if (auto terr = std::dynamic_pointer_cast<eterrain2d::Sprite>(spr))
	//{
	//	eterrain2d::EditDialog dlg(m_wnd, canvas->GetGLContext(), terr, m_sprites_impl);
	//	dlg.ShowModal();
	//} 
	//else if (auto skeleton = std::dynamic_pointer_cast<libskeleton::Sprite>(spr)) 
	//{
	//	libskeleton::EditDialog dlg(m_wnd, canvas->GetGLContext(), skeleton);
	//	dlg.ShowModal();
	//}
	else if (auto shape = std::dynamic_pointer_cast<eshape::Sprite>(spr))
	{
		eshape::EditDialogSimple dlg(m_wnd, canvas->GetGLContext(), shape, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (auto node3 = std::dynamic_pointer_cast<enode3d::Sprite>(spr))
	{
		enode3d::EditDialog dlg(m_wnd, canvas->GetGLContext(), node3);
		dlg.ShowModal();
	}
	else if (auto audio = std::dynamic_pointer_cast<eaudio::Sprite>(spr))
	{
		stop_audio = false;
		audio->Play();
	}

	m_sprites_impl->EnableObserve(true);
	m_stage->EnableObserve(true);
	m_stage->SetActive(true);
	canvas->EnableObserve(true);
	canvas->SetDrawable(true);
	if (m_viewlist) {
		m_viewlist->EnableObserve(true);
	}

	if (edited_sym && edited_sym->IsEditDirty()) {
		ee::SetWndDirtySJ::Instance()->SetDirty();
		std::dynamic_pointer_cast<ee::Symbol>(edited_sym)->SetEditDirty(false);
	}

	ee::CurrSprTreePath::Instance()->Pop();

	if (stop_audio) {
		gum::Audio::Instance()->Stop();
	}
}

}