#include "SpritesPanelImpl.h"
#include "IDataContainer.h"
#include "EditPanel.h"
#include "SpriteDropTarget.h"
#include "SpritesContainer.h"
#include "IStageCanvas.h"

#include "dataset/ISprite.h"

namespace d2d
{

SpritesPanelImpl::SpritesPanelImpl(EditPanel* stage, IDataContainer* container)
	: MultiSpritesImpl(stage->GetStageImpl())
	, m_stage(stage)
{
	m_container = container;
	m_container->Retain();
}

SpritesPanelImpl::SpritesPanelImpl(EditPanel* stage, LibraryPanel* library)
	: MultiSpritesImpl(stage->GetStageImpl())
	, m_stage(stage)
{
	m_stage->SetDropTarget(new SpriteDropTarget(this, stage, library));

	m_container = new SpritesContainer;
}

SpritesPanelImpl::~SpritesPanelImpl()
{
	m_container->Release();
}

bool SpritesPanelImpl::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	MultiSpritesImpl::ReorderSprite(sprite, up);

	bool ret = m_container->ResetOrder(sprite, up);
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

bool SpritesPanelImpl::InsertSprite(ISprite* sprite)
{
	MultiSpritesImpl::InsertSprite(sprite);

	bool ret = m_container->Insert(sprite);
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

bool SpritesPanelImpl::RemoveSprite(ISprite* sprite)
{
	MultiSpritesImpl::RemoveSprite(sprite);

	bool ret = m_container->Remove(sprite);
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

bool SpritesPanelImpl::ClearAllSprite()
{
	MultiSpritesImpl::ClearAllSprite();

	bool ret = m_container->Clear();
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

void SpritesPanelImpl::TraverseSprites(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/,
									   bool order/* = true*/) const
{
	m_container->Traverse(visitor, type, order);
}

}