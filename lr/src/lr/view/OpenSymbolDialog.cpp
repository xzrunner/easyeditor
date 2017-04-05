#include "OpenSymbolDialog.h"
#include "StagePanel.h"
#include "LibraryPanel.h"
#include "UnitEditDlg.h"
#include "LevelEditDlg.h"

#include "dataset/ShapesUD.h"
#include "dataset/Layer.h"
#include "dataset/data_utility.h"

#include <ee/EditPanelImpl.h>
#include <ee/FontBlankSprite.h>
#include <ee/TextDialog.h>
#include <ee/SpriteDialog.h>
#include <ee/panel_msg.h>
#include <ee/CurrSprTreePath.h>

#include <easyscale9.h>
#include <easymesh.h>
#include <easyanim.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyshape.h>
#include <easyterrain2d.h>
#include <easyshadow.h>

namespace lr
{

OpenSymbolDialog::OpenSymbolDialog(wxWindow* wnd, ee::EditPanelImpl* stage, 
								   ee::MultiSpritesImpl* sprites_impl)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
{
}

void OpenSymbolDialog::Open(ee::Sprite* spr)
{
	if (dynamic_cast<ee::Symbol*>(spr->GetSymbol())->GetFilepath().find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	ee::CurrSprTreePath::Instance()->Push(spr);

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	m_sprites_impl->EnableObserve(false);
	stage->EnableObserve(false);
	m_stage->SetActive(false);
	stage->GetCanvas()->EnableObserve(false);
	stage->GetCanvas()->SetDrawable(false);

	const ee::Symbol* edited_sym = NULL;
	if (static_cast<LibraryPanel*>(stage->GetLibrary())->IsCurrUnitLayer()) 
	{
		std::vector<std::string> path_names;
		static_cast<LibraryPanel*>(stage->GetLibrary())->GetAllPathName(path_names);
		UnitEditDlg dlg(m_wnd, spr, path_names);
		if (dlg.ShowModal() == wxID_OK) {
			spr->SetTag(dlg.ToString());
		}
	} 
	else if (static_cast<LibraryPanel*>(stage->GetLibrary())->IsCurrLevelLayer())  {
		LevelEditDlg dlg(m_wnd, spr);
		if (dlg.ShowModal() == wxID_OK) {
			bool use_symbol = spr->GetTag().find("[symbol]") != std::string::npos;
			std::string tag;
			if (use_symbol) {
				tag = "[symbol];";
			}
			tag += dlg.ToString();
			spr->SetTag(tag);
		}
	}
	else if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr))
	{
		ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
		edited_sym = sym;
		ecomplex::EditDialog dlg(m_wnd, sym, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();

		//////////////////////////////////////////////////////////////////////////

		// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
		// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(spr))
	{
		libanim::Symbol* sym = dynamic_cast<libanim::Symbol*>(anim->GetSymbol());
		edited_sym = sym;
		libanim::PreviewDialog dlg(m_wnd, sym, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(spr))
	{
		escale9::Symbol* sym = dynamic_cast<escale9::Symbol*>(patch9->GetSymbol());
		edited_sym = sym;
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
		UpdateShapeFromETexture(tex);
	}
	else if (eshape::Sprite* shape = dynamic_cast<eshape::Sprite*>(spr))
	{
		eshape::EditDialogSimple dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), shape, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(spr))
	{
		eterrain2d::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), terr, m_sprites_impl);
		dlg.ShowModal();
	} 
	else if (eshadow::Sprite* shadow = dynamic_cast<eshadow::Sprite*>(spr))
	{
		eshadow::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), shadow, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (spr)
	{
		ee::SpriteDialog dlg(m_wnd, spr);
		if (dlg.ShowModal() == wxID_OK) {
			spr->SetName(dlg.GetNameStr());
			spr->SetTag(dlg.GetTagStr());
		}
	}

	m_sprites_impl->EnableObserve(true);
	stage->EnableObserve(true);
	m_stage->SetActive(true);
	stage->GetCanvas()->EnableObserve(true);
	stage->GetCanvas()->SetDrawable(true);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	if (edited_sym && edited_sym->IsEditDirty()) {
		ee::SetWndDirtySJ::Instance()->SetDirty();
		const_cast<ee::Symbol*>(edited_sym)->SetEditDirty(false);
	}

	ee::CurrSprTreePath::Instance()->Pop();
}

void OpenSymbolDialog::UpdateShapeFromETexture(etexture::Sprite* spr)
{
	if (!spr->GetUserData()) {
		return;
	}

	UserData* ud = static_cast<UserData*>(spr->GetUserData());
	if (ud->type == UT_BASE_FILE) {
		return;
	}

	ShapesUD* sud = static_cast<ShapesUD*>(ud);
	LibraryPanel* library = static_cast<LibraryPanel*>(static_cast<StagePanel*>(m_wnd)->GetLibrary());
	Layer* layer = library->GetLayer(sud->layer_id);
	for (int i = 0, n = sud->shape_names.size(); i < n; ++i) {
		ee::Shape* shape = layer->QueryShape(sud->shape_names[i]);
		layer->RemoveShape(shape);
	}

	sud->shape_names.clear();
	std::vector<ee::Shape*> shapes;
	create_shapes_from_etxture(spr, shapes);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		layer->InsertShape(shapes[i]);
		sud->shape_names.push_back(shapes[i]->GetName());
	}
}

}