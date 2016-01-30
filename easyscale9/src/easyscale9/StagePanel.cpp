#include "StagePanel.h"
#include "StageCanvas.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <easyscale9.h>

namespace escale9
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(GetStageImpl())
	, m_symbol(NULL)
	, m_library(library)
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));

	memset(m_sprites, 0, sizeof(int) * 9);

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(d2d::InsertSpriteSJ::Instance());
	RegistSubject(d2d::RemoveSpriteSJ::Instance());
	RegistSubject(d2d::ClearSpriteSJ::Instance());
}

void StagePanel::TraverseSprites(d2d::Visitor& visitor, d2d::DataTraverseType type/* = d2d::e_allExisting*/, 
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

	if (type == e_3GridHor) {
		height = m_sprites[1][1]->GetSymbol().GetSize().Height();
	} else if (type == e_3GridVer) {
		width = m_sprites[1][1]->GetSymbol().GetSize().Width();
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
	d2d::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_INSERT_SPRITE:
		{
			d2d::InsertSpriteSJ::Params* p = (d2d::InsertSpriteSJ::Params*)ud;
			Insert(p->spr);
		}
		break;
	case d2d::MSG_REMOVE_SPRITE:
		Remove((d2d::Sprite*)ud);
		break;
	case d2d::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void StagePanel::Insert(d2d::Sprite* spr)
{
	const d2d::Vector& pos = spr->GetPosition();
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

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(d2d::Sprite* spr)
{
	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j) 
		{
			if (m_sprites[i][j] == spr)
			{
				m_sprites[i][j] = NULL;
				spr->Release();
				d2d::SetCanvasDirtySJ::Instance()->SetDirty();
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

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

}

} // escale9