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
	, m_sym(NULL)
	, m_library(library)
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));

	memset(m_sprs, 0, sizeof(int) * 9);

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());
}

void StagePanel::TraverseSprites(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type/* = ee::e_allExisting*/, 
								 bool order/* = true*/) const
{
	for (int i = 0; i < 9; ++i) {
		if (m_sprs[i]) {
			bool next;
			visitor.Visit(dynamic_cast<ee::Sprite*>(m_sprs[i]), next);
			if (!next) return;
		}
	}
}

ee::Sprite* StagePanel::getSprite(int row, int col) 
{
	if (row < 0 || row >= 3 || col < 0 || col >= 3) {
		return NULL;
	} else {
		return dynamic_cast<ee::Sprite*>(m_sprs[row * 3 + col]);
	}
}

void StagePanel::rebuildPatchSymbol()
{
	s2::SCALE9_TYPE type = s2::Scale9::CheckType(m_sprs);
	if (type == s2::S9_NULL) {
		return;
	}

	if (!m_sym) {
		m_sym = new Symbol;
	}

	float width = m_toolbar->getWidth(),
		  height = m_toolbar->getHeight();
	s2::Scale9& s9 = const_cast<s2::Scale9&>(dynamic_cast<s2::Scale9Symbol*>(m_sym)->GetScale9());
	s9.Build(type, width, height, m_sprs, 0, 0, 0, 0);

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

	if (m_sprs[row * 3 + col] && m_sprs[row * 3 + col] != spr) {
		m_sprs[row * 3 + col]->RemoveReference();
	}
	spr->AddReference();
	m_sprs[row * 3 + col] = spr;

	spr->SetPosition(ComposeGrids::GetGridCenter(col, row));

	rebuildPatchSymbol();

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(ee::Sprite* spr)
{
	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j) 
		{
			if (m_sprs[i * 3 + j] == spr)
			{
				m_sprs[i * 3 + j] = NULL;
				spr->RemoveReference();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
				return;
			}
		}
	}
}

void StagePanel::Clear()
{
	for (int i = 0; i < 9; ++i) {
		if (m_sprs[i]) {
			m_sprs[i]->RemoveReference();
		}
	}
	memset(m_sprs, 0, sizeof(int) * 9);

	delete m_sym, m_sym = NULL;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

}

} // escale9