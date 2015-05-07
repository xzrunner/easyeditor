#include "SpritesPanelImpl.h"
#include "IDataContainer.h"

#include "EditPanel.h"
#include "SpriteDropTarget.h"
#include "SpritesContainer.h"

namespace d2d
{
	SpritesPanelImpl::SpritesPanelImpl(wxWindow* parent, IDataContainer* container)
		: MultiSpritesImpl(parent)
	{
		m_container = container;
		m_container->Retain();
	}

	SpritesPanelImpl::SpritesPanelImpl(EditPanel* editPanel, LibraryPanel* libraryPanel)
		: MultiSpritesImpl(editPanel)
	{
		editPanel->SetDropTarget(new SpriteDropTarget(this, editPanel, libraryPanel));

		m_container = new SpritesContainer;
	}

	SpritesPanelImpl::~SpritesPanelImpl()
	{
		m_container->Release();
	}

	void SpritesPanelImpl::traverseSprites(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/,
		bool order/* = true*/) const
	{
		m_container->Traverse(visitor, type, order);
	}

	void SpritesPanelImpl::removeSprite(ISprite* sprite)
	{
		m_container->Remove(sprite);
	}

	void SpritesPanelImpl::insertSprite(ISprite* sprite)
	{
		m_container->Insert(sprite);
		refresh();
	}

	void SpritesPanelImpl::clearSprites()
	{
		m_container->Clear();
	}

	void SpritesPanelImpl::resetSpriteOrder(d2d::ISprite* sprite, bool up)
	{
		m_container->ResetOrder(sprite, up);
	}
}