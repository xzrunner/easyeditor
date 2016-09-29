#include "OpenSymbolDialog.h"
#include "EditDialog.h"
#include "Sprite.h"
#include "Symbol.h"
#include "OpenSymbolLsn.h"

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

#include <ee/EditPanelImpl.h>
#include <ee/FontBlankSprite.h>
#include <ee/TextDialog.h>
#include <ee/ViewlistPanel.h>

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

void OpenSymbolDialog::Open(ee::Sprite* spr, ee::CrossGuides* guides)
{
	if (!spr) {
		return;
	}

	if (dynamic_cast<ee::Symbol*>(spr->GetSymbol())->GetFilepath().find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	m_sprites_impl->EnableObserve(false);
	m_stage->EnableObserve(false);
	m_stage->GetCanvas()->EnableObserve(false);
	m_stage->GetCanvas()->SetDrawable(false);
	if (m_viewlist) {
		m_viewlist->EnableObserve(false);
	}
	for (int i = 0, n = m_lsns.size(); i < n; ++i) {
		m_lsns[i]->BeforeOpen();
	}

	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr))
	{
		ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
 		EditDialog dlg(m_wnd, sym, m_stage->GetCanvas()->GetGLContext(), guides);
 		dlg.ShowModal();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (eanim::Sprite* anim = dynamic_cast<eanim::Sprite*>(spr))
	{
		const eanim::Symbol* sym = dynamic_cast<const eanim::Symbol*>(anim->GetSymbol());
		eanim::PreviewDialog dlg(m_wnd, sym, m_stage->GetCanvas()->GetGLContext());
 		dlg.ShowModal();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(spr))
 	{
		escale9::Symbol* sym = dynamic_cast<escale9::Symbol*>(patch9->GetSymbol());
  		escale9::EditDialog dlg(m_wnd, sym, m_stage->GetCanvas()->GetGLContext());
  		dlg.ShowModal();
 	}
	else if (emesh::Sprite* mesh = dynamic_cast<emesh::Sprite*>(spr))
	{
		emesh::EditDialog dlg(m_wnd, mesh, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(spr))
	{
		ee::TextDialog dlg(m_wnd, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr))
	{
		etexture::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), tex, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(spr))
	{
		eicon::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), spr, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(spr))
	{
		etext::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), text, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(spr))
	{
		eterrain2d::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), terr, m_sprites_impl);
		dlg.ShowModal();
	} 
	else if (libskeleton::Sprite* skeleton = dynamic_cast<libskeleton::Sprite*>(spr)) 
	{
		libskeleton::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), skeleton);
		dlg.ShowModal();
	}

	m_sprites_impl->EnableObserve(true);
	m_stage->EnableObserve(true);
	m_stage->GetCanvas()->EnableObserve(true);
	m_stage->GetCanvas()->SetDrawable(true);
	if (m_viewlist) {
		m_viewlist->EnableObserve(true);
	}
	for (int i = 0, n = m_lsns.size(); i < n; ++i) {
		m_lsns[i]->AfterOpen();
	}
}

}