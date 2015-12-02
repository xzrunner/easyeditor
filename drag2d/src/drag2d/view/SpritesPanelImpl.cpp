#include "SpritesPanelImpl.h"
#include "IDataContainer.h"
#include "EditPanelImpl.h"
#include "SpriteDropTarget.h"
#include "SpritesContainer.h"
#include "IStageCanvas.h"

#include "dataset/ISprite.h"
#include "message/subject_id.h"
#include "message/ReorderSpriteSJ.h"
#include "message/ReorderSpriteMostSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSpriteSJ.h"
#include "message/SetCanvasDirtySJ.h"
#include "render/SpatialIndex.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

SpritesPanelImpl::SpritesPanelImpl(EditPanelImpl* stage, IDataContainer* container)
	: MultiSpritesImpl(stage)
	, m_stage(stage)
{
	m_container = container;
	m_container->Retain();

	InitSubjects();
}

SpritesPanelImpl::SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library)
	: MultiSpritesImpl(stage)
	, m_stage(stage)
{
	m_stage->SetDropTarget(new SpriteDropTarget(stage, library));
	m_container = new SpritesContainer;

	InitSubjects();
}

SpritesPanelImpl::~SpritesPanelImpl()
{
	m_container->Release();
}

void SpritesPanelImpl::TraverseSprites(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/,
									   bool order/* = true*/) const
{
	m_container->Traverse(visitor, type, order);
}

void SpritesPanelImpl::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

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
	case MSG_INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			m_container->Insert(p->spr, p->idx);
#ifdef OPEN_SCREEN_CACHE
			SpatialIndex::Instance()->Insert(p->spr);
			SpriteRenderer::Instance()->InvalidRect(p->spr);
#endif // OPEN_SCREEN_CACHE
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_REMOVE_SPRITE:
		{
			ISprite* spr = (ISprite*)ud;
			m_container->Remove(spr);
#ifdef OPEN_SCREEN_CACHE
			SpatialIndex::Instance()->Remove(spr);
			SpriteRenderer::Instance()->InvalidRect(spr);
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
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());
}

}