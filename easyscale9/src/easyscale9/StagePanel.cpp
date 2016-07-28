#include "StagePanel.h"
#include "StageCanvas.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <easyscale9.h>

#include <ee/StageDropTarget.h>
#include <ee/sprite_msg.h>
#include <ee/panel_msg.h>
#include <ee/subject_id.h>
#include <ee/Visitor.h>

namespace escale9
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
	, m_symbol(NULL)
	, m_library(library)
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));

	memset(m_sprites, 0, sizeof(int) * 9);

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());
}

void StagePanel::TraverseSprites(ee::Visitor& visitor, ee::DataTraverseType type/* = ee::e_allExisting*/, 
								 bool order/* = true*/) const
{
	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;

			bool next;
			visitor.Visit(m_sprites[i][j], next);
			if (!next) return;
		}
	}
}

void StagePanel::rebuildPatchSymbol()
{
	Scale9Type type = Scale9Data::CheckType(m_sprites);
	if (type == e_null) {
		return;
	}

	if (m_symbol) delete m_symbol;
	m_symbol = new Symbol;

	float width = m_toolbar->getWidth(),
		  height = m_toolbar->getHeight();

	sm::vec2 sz = m_sprites[1][1]->GetSymbol().GetSize().Size();
	if (type == e_3GridHor) {
		height = sz.y;
	} else if (type == e_3GridVer) {
		width = sz.x;
	}

	m_symbol->ComposeFromSprites(m_sprites, width, height);

	m_toolbar->setSize(width, height);
}

void StagePanel::setToolbarPanel(ToolbarPanel* toolbar)
{
	m_toolbar = toolbar;
	static_cast<StageCanvas*>(GetCanvas())->setToolbarPanel(toolbar);
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			Insert(p->spr);
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		Remove((ee::Sprite*)ud);
		break;
	case ee::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void StagePanel::Insert(ee::Sprite* spr)
{
	const sm::vec2& pos = spr->GetPosition();
	int col, row;
	ComposeGrids::Query(pos, &col, &row);
	if (col == -1 || row == -1) {
		return;
	}

	if (m_sprites[row][col] && m_sprites[row][col] != spr) {
		m_sprites[row][col]->Release();
	}
	spr->Retain();
	m_sprites[row][col] = spr;

	spr->SetTransform(ComposeGrids::GetGridCenter(col, row), spr->GetAngle());

	rebuildPatchSymbol();

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(ee::Sprite* spr)
{
	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j) 
		{
			if (m_sprites[i][j] == spr)
			{
				m_sprites[i][j] = NULL;
				spr->Release();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
				return;
			}
		}
	}
}

void StagePanel::Clear()
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->Release();
		}
	}
	memset(m_sprites, 0, sizeof(int) * 9);

	delete m_symbol, m_symbol = NULL;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

}

} // escale9