#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "dataset/ShapesUD.h"
#include "dataset/Layer.h"
#include "dataset/data_utility.h"
#include "view/LibraryPanel.h"
#include "view/UnitEditDlg.h"
#include "frame/config.h"

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

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(stage_wnd, stage, spritesImpl, callback)
{
	stage->SetCursor(wxCursor(wxCURSOR_PENCIL));

	m_first_press.setInvalid();
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->PointQuery(pos);

	if (m_first_press.isValid()) {
		stage->Pathfinding(m_first_press, pos);
		m_first_press.setInvalid();
	} else {
		m_first_press = pos;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (!selected) {
		return false;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->SetUpdateState(false);
	if (static_cast<LibraryPanel*>(stage->GetLibrary())->IsCurrUnitLayer()) 
	{
		std::vector<std::string> path_names;
		static_cast<LibraryPanel*>(stage->GetLibrary())->GetAllPathName(path_names);
		UnitEditDlg dlg(m_wnd, selected, path_names);
		if (dlg.ShowModal() == wxID_OK) {
			selected->tag = dlg.ToString();
		}
	} 
	else if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
 		ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->GetSymbol());
 		ecomplex::EditDialog dlg(m_wnd, &symbol);
 		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(selected))
	{
 		libanim::PreviewDialog dlg(m_wnd, &anim->GetSymbol());
 		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(selected))
 	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->GetSymbol());
  		escale9::EditDialog dlg(m_wnd, &symbol);
  		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_wnd, sprite);
		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();
	}
	else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(selected))
	{
		d2d::TextDialog dlg(m_wnd, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(selected))
	{
		etexture::EditDialog dlg(m_wnd, tex, m_spritesImpl);
		dlg.ShowModal();
		UpdateShapeFromETexture(tex);
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();
	}
	else if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(selected))
	{
		libshape::EditDialogSimple dlg(m_wnd, shape, m_spritesImpl);
		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();		
	}
	else if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(selected))
	{
		eterrain2d::EditDialog dlg(m_wnd, terr, m_spritesImpl);
		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();
	} 
	else if (eshadow::Sprite* shadow = dynamic_cast<eshadow::Sprite*>(selected))
	{
		eshadow::EditDialog dlg(m_wnd, shadow, m_spritesImpl);
		dlg.ShowModal();
		m_stage->SetCanvasDirty();
		m_stage->RefreshFrame();
		m_stage->ResetViewport();
	}
	else if (selected)
	{
		d2d::SpriteDialog dlg(m_wnd, selected);
		if (dlg.ShowModal() == wxID_OK) {
			selected->name = dlg.GetNameStr();
			selected->tag = dlg.GetTagStr();
		}
	}

	stage->SetUpdateState(true);

	return false;
}

void SelectSpritesOP::UpdateShapeFromETexture(etexture::Sprite* spr)
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
		d2d::IShape* shape = layer->QueryShape(sud->shape_names[i]);
		layer->RemoveShape(shape);
	}

	sud->shape_names.clear();
	std::vector<d2d::IShape*> shapes;
	create_shapes_from_etxture(spr, shapes);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		layer->InsertShape(shapes[i]);
		sud->shape_names.push_back(shapes[i]->name);
	}
}

}