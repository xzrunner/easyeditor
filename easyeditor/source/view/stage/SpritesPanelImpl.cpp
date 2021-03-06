#include "SpritesPanelImpl.h"
#include "DataContainer.h"
#include "EditPanelImpl.h"
#include "SpriteDropTarget.h"
#include "SpritesContainer.h"
#include "StageCanvas.h"
#include "Sprite.h"
#include "subject_id.h"
#include "sprite_msg.h"
#include "panel_msg.h"
#include "EE_SP.h"
#include "SpriteRenderer.h"

namespace ee
{

SpritesPanelImpl::SpritesPanelImpl(EditPanelImpl* stage)
	: MultiSpritesImpl(stage)
	, m_stage(stage)
{
	InitSubjects();
}

SpritesPanelImpl::SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library)
	: MultiSpritesImpl(stage)
	, m_stage(stage)
{
	m_stage->SetDropTarget(new SpriteDropTarget(stage, library));
	m_container = std::make_shared<SpritesContainer>();

	InitSubjects();
}

void SpritesPanelImpl::TraverseSprites(RefVisitor<Sprite>& visitor, DataTraverseType type/* = e_allExisting*/,
									   bool order/* = true*/) const
{
	if (m_container) {
		m_container->Traverse(visitor, type, order);
	}
}

void SpritesPanelImpl::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

	if (!m_container) {
		return;
	}

	switch (sj_id)
	{
	case MSG_REORDER_SPRITE:
		{
			ReorderSpriteSJ::Params* p = (ReorderSpriteSJ::Params*)ud;
			m_container->ResetOrder(p->spr, p->up);
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_REORDER_SPRITE_MOST:
		{
			ReorderSpriteMostSJ::Params* p = (ReorderSpriteMostSJ::Params*)ud;
			m_container->ResetOrderMost(p->spr, p->up);
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_SORT_SPRITES:
		{
			std::vector<ee::SprPtr>& sprs = *(std::vector<ee::SprPtr>*)ud;
			std::static_pointer_cast<SpritesContainer>(m_container)->Sort(sprs);
		}
		break;
	case MSG_INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			m_container->Insert(p->spr, p->idx);
#ifdef OPEN_SCREEN_CACHE
			SpatialPartition::Instance()->Insert(p->spr);
			SpriteRenderer::InvalidRect(p->spr);
#endif // OPEN_SCREEN_CACHE
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_REMOVE_SPRITE:
		{
			auto spr = *(SprPtr*)ud;
			m_container->Remove(spr);
#ifdef OPEN_SCREEN_CACHE
			SpatialPartition::Instance()->Remove(spr);
			SpriteRenderer::InvalidRect(spr);
#endif // OPEN_SCREEN_CACHE
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_CLEAR_SPRITE:
		{
			m_container->Clear();
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

void SpritesPanelImpl::InitSubjects()
{
	RegistSubject(ReorderSpriteSJ::Instance());
	RegistSubject(ReorderSpriteMostSJ::Instance());
	RegistSubject(SortSpriteSJ::Instance());
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());
}

}