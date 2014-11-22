#include "SpritesPanelImpl.h"

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

	void SpritesPanelImpl::traverseSprites(IVisitor& visitor, TraverseType type/* = e_allExisting*/,
		bool order/* = true*/) const
	{
		m_container->traverse(visitor, order);
	}

	void SpritesPanelImpl::removeSprite(ISprite* sprite)
	{
		m_container->remove(sprite);
	}

	void SpritesPanelImpl::insertSprite(ISprite* sprite)
	{
		m_container->insert(sprite);
		refresh();
	}

	void SpritesPanelImpl::clearSprites()
	{
		m_container->clear();
	}

	void SpritesPanelImpl::resetSpriteOrder(d2d::ISprite* sprite, bool up)
	{
		m_container->resetOrder(sprite, up);
	}
}