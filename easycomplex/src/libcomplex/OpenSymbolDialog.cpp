#include "OpenSymbolDialog.h"
#include "EditDialog.h"
#include "Sprite.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyscale9.h>
#include <easyanim.h>
#include <easytexture.h>
#include <easyicon.h>
#include <easyshadow.h>
#include <easytext.h>

namespace ecomplex
{

OpenSymbolDialog::OpenSymbolDialog(wxWindow* wnd, d2d::EditPanelImpl* stage,
								   d2d::MultiSpritesImpl* sprites_impl)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
{
}

void OpenSymbolDialog::Open(d2d::ISprite* spr)
{
	if (!spr) {
		return;
	}

	if (spr->GetSymbol().GetFilepath().find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	d2d::RenderContext* context = d2d::RenderContext::GetCurrContext();

	d2d::Vector last_offset;
	float last_scale;
	context->GetModelView(last_offset, last_scale);

	int last_w, last_h;
	context->GetProjection(last_w, last_h);

	m_sprites_impl->EnableObserve(false);
	m_stage->GetCanvas()->EnableObserve(false);

	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr))
	{
 		Symbol& symbol = const_cast<Symbol&>(complex->GetSymbol());

 		EditDialog dlg(m_wnd, &symbol);
 		dlg.ShowModal();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(spr))
	{
 		libanim::PreviewDialog dlg(m_wnd, &anim->GetSymbol());
 		dlg.ShowModal();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(spr))
 	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->GetSymbol());
  		escale9::EditDialog dlg(m_wnd, &symbol);
  		dlg.ShowModal();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(spr))
	{
		emesh::EditDialog dlg(m_wnd, sprite);
		dlg.ShowModal();
	}
	else if (d2d::FontBlankSprite* font = dynamic_cast<d2d::FontBlankSprite*>(spr))
	{
		d2d::TextDialog dlg(m_wnd, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr))
	{
		etexture::EditDialog dlg(m_wnd, tex, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(spr))
	{
		eicon::EditDialog dlg(m_wnd, spr, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (etext::Sprite* text = dynamic_cast<etext::Sprite*>(spr))
	{
		etext::EditDialog dlg(m_wnd, text, m_sprites_impl);
		dlg.ShowModal();
	}

	m_sprites_impl->EnableObserve(true);
	m_stage->GetCanvas()->EnableObserve(true);

	m_stage->GetCanvas()->SetCurrentCanvas();

	context->SetModelView(last_offset, last_scale);
	context->SetProjection(last_w, last_h);		
}

}