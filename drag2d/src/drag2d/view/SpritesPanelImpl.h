#pragma once

#include "MultiSpritesImpl.h"

namespace d2d
{
	class EditPanel;
	class LibraryPanel;
	class ISprite;
	class IDataContainer;

	class SpritesPanelImpl : public MultiSpritesImpl
	{
	public:
		SpritesPanelImpl(wxWindow* parent, IDataContainer* container);
		SpritesPanelImpl(EditPanel* editPanel, LibraryPanel* libraryPanel);
		virtual ~SpritesPanelImpl();

		//
		// MultiSpritesImpl interface
		//
		virtual void traverseSprites(IVisitor& visitor, 
			DataTraverseType type = DT_ALL, bool order = true) const;
		virtual void removeSprite(ISprite* sprite);
		virtual void insertSprite(ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

	private:
		IDataContainer* m_container;

	}; // SpritesPanelImpl
}