#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteImpl.h"
#include "TopPannels.h"
#include "TopLibraryPanel.h"
#include "ToolbarPanel.h"
#include "TopToolbarPanel.h"
#include "PreviewDialog.h"
#include "Code.h"
#include "SymbolCfg.h"
#include "SpriteDropTarget.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/FetchAllVisitor.h>
#include <ee/sprite_msg.h>
#include <ee/panel_msg.h>
#include <ee/SpriteSelection.h>
#include <ee/subject_id.h>
#include <ee/SpriteDropTarget.h>
#include <ee/ViewlistPanel.h>

#include <easyui.h>
#include <easycomplex.h>
#include <easybuilder.h>

namespace eui
{
namespace window
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
	, m_sym(new Symbol())
	, m_top_pannels(top_pannels)
	, m_symbols_cfg(this, top_pannels->library->GetUILibrary())
{
	SetDropTarget(new SpriteDropTarget(GetStageImpl(), top_pannels->library->GetUILibrary()));

	ee::ArrangeSpriteOP<ecomplex::SelectSpritesOP>* edit_op = new ee::ArrangeSpriteOP<ecomplex::SelectSpritesOP>(
		this, GetStageImpl(), this, top_pannels->property, NULL, ee::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, top_pannels->property));
	static_cast<ecomplex::SelectSpritesOP*>(edit_op)->SetGuides(&m_guides);
	SetEditOP(edit_op);
	edit_op->Release();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->Release();

	int w, h;
	QueryWindowViewSizeSJ::Instance()->Query(w, h);
	m_sym->SetWidth(w);
	m_sym->SetHeight(h);

	m_toolbar = new ToolbarPanel(top_pannels->toolbar, this);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	InitSubjects();
}

StagePanel::~StagePanel()
{
	m_sym->Release();
}

void StagePanel::LoadFromFile(const char* filename)
{
	FileIO::Load(filename, m_sym);
	m_toolbar->SetWindowName(m_sym->name);

	std::vector<ee::Sprite*> sprites;
	m_sym->GetAnchorMgr().Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	const std::vector<Sprite*>& ref_sprites = m_sym->GetExtRefs();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_top_pannels->viewlist->Insert(sprites[i]);
	}
	for (int i = 0, n = ref_sprites.size(); i < n; ++i) {
		m_top_pannels->viewlist->Insert(ref_sprites[i]);
	}	
}

void StagePanel::StoreToFile(const char* filename) const
{
	FileIO::Store(filename, m_sym);
}

void StagePanel::EnablePage(bool enable)
{
	if (enable) {
		m_top_pannels->toolbar->EnableToolbar(m_toolbar_idx);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		m_top_pannels->library->EnableUILibrary(true);
	} else {
		GetSpriteSelection()->Clear();
	}

	m_toolbar->EnableObserve(enable);
	EnableObserve(enable);
	GetStageImpl()->EnableObserve(enable);
}

void StagePanel::OnPreview()
{
	int width, height;
	QueryWindowViewSizeSJ::Instance()->Query(width, height);

	std::vector<const ee::Sprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<const ee::Sprite>(sprites));

	EnableObserve(false);
	GetCanvas()->EnableObserve(false);
	GetStageImpl()->EnableObserve(false);

	PreviewDialog dlg(const_cast<StagePanel*>(this), GetCanvas()->GetGLContext(), width, height, sprites);
	dlg.ShowModal();

	EnableObserve(true);
	GetCanvas()->EnableObserve(true);
	GetStageImpl()->EnableObserve(true);
}

void StagePanel::OnCode() const
{
	ebuilder::CodeDialog dlg(const_cast<StagePanel*>(this));
	std::vector<ee::Sprite*> sprites;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));

	// ui
	{
		ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, "ui.lua");

		ebuilder::CodeGenerator gen;
		Code code(gen);
		code.ResolveUI(sprites);
		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		dlg.notebook->AddPage(page, page->getName());
	}
	// tid
	{
		ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, "texts.lua");

		ebuilder::CodeGenerator gen;
		Code code(gen);
		code.ResolveText(sprites);
		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		dlg.notebook->AddPage(page, page->getName());
	}
	dlg.ShowModal();
}

void StagePanel::TraverseSprites(ee::Visitor& visitor, ee::DataTraverseType type, bool order) const
{
	m_sym->Traverse(visitor);
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

void StagePanel::SetViewSize(int width, int height)
{
	if (m_sym->GetWidth() == width && 
		m_sym->GetHeight() == height) {
		return;
	}

	m_sym->SetWidth(width);
	m_sym->SetHeight(height);

	m_sym->GetAnchorMgr().OnViewChanged(width, height);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case MSG_QUERY_WINDOW_VIEW_SIZE:
		if (m_sym) {
			QueryWindowViewSizeSJ::Params* p = (QueryWindowViewSizeSJ::Params*)ud;
			p->width = m_sym->GetWidth();
			p->height = m_sym->GetHeight();
		}
		break;
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			ee::Sprite* spr = p->spr;
			if (Sprite* spr_ui = dynamic_cast<Sprite*>(spr)) {
				m_sym->InsertExtRef(spr_ui);
			} else {
				std::string type = "";
				SymbolCfg::Instance()->QueryType(&spr->GetSymbol(), type);
				if (type.empty() && spr->GetTag().find("type=unknown") == std::string::npos) {
					spr->SetTag("type=unknown;" + spr->GetTag());
				}
				m_sym->GetAnchorMgr().Insert(spr);
			}
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		{
			ee::Sprite* spr = (ee::Sprite*)ud;
			if (Sprite* spr_ui = dynamic_cast<Sprite*>(spr)) {
				m_sym->RemoveExtRef(spr_ui);
			} else {
				m_sym->GetAnchorMgr().Remove(spr);
			}
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_CLEAR_SPRITE:
		m_sym->GetAnchorMgr().Clear();
		m_sym->ClearExtRef();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		break;
	case ee::MSG_REORDER_SPRITE:
		{
			ee::ReorderSpriteSJ::Params* p = (ee::ReorderSpriteSJ::Params*)ud;
			if (Sprite* spr_ui = dynamic_cast<Sprite*>(p->spr)) {
				m_sym->ResetExtRefOrder(spr_ui, p->up);
			} else {
				m_sym->GetAnchorMgr().ResetOrder(p->spr, p->up);
			}
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case ee::MSG_REORDER_SPRITE_MOST:
		{
			ee::ReorderSpriteMostSJ::Params* p = (ee::ReorderSpriteMostSJ::Params*)ud;
			if (Sprite* spr_ui = dynamic_cast<Sprite*>(p->spr)) {
				m_sym->ResetExtRefOrderMost(spr_ui, p->up);
			} else {
				m_sym->GetAnchorMgr().ResetOrderMost(p->spr, p->up);
			}
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

void StagePanel::InitSubjects()
{
	RegistSubject(ee::ReorderSpriteSJ::Instance());
	RegistSubject(ee::ReorderSpriteMostSJ::Instance());
	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());
}

}
}