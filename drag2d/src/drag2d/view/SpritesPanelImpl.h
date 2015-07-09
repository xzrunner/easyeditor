#ifndef _DRAG2D_SPRITES_PANEL_IMPL_H_
#define _DRAG2D_SPRITES_PANEL_IMPL_H_

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
	SpritesPanelImpl(EditPanel* stage, IDataContainer* container);
	SpritesPanelImpl(EditPanel* stage, LibraryPanel* library);
	virtual ~SpritesPanelImpl();

	//
	// MultiSpritesImpl interface
	//
	virtual bool ReorderSprite(d2d::ISprite* sprite, bool up);
	virtual bool InsertSprite(ISprite* sprite);
	virtual bool RemoveSprite(ISprite* sprite);
	virtual bool ClearAllSprite();
	virtual void TraverseSprites(IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const;

private:
	EditPanel* m_stage;

	IDataContainer* m_container;

}; // SpritesPanelImpl

}

#endif // _DRAG2D_SPRITES_PANEL_IMPL_H_