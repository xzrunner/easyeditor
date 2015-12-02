#ifndef _DRAG2D_SPRITES_PANEL_IMPL_H_
#define _DRAG2D_SPRITES_PANEL_IMPL_H_

#include "MultiSpritesImpl.h"

namespace d2d
{

class EditPanelImpl;
class LibraryPanel;
class ISprite;
class IDataContainer;

class SpritesPanelImpl : public MultiSpritesImpl
{
public:
	SpritesPanelImpl(EditPanelImpl* stage, IDataContainer* container);
	SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library);
	virtual ~SpritesPanelImpl();

	//
	// MultiSpritesImpl interface
	//
	virtual void TraverseSprites(IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitSubjects();

private:
	EditPanelImpl* m_stage;

	IDataContainer* m_container;

}; // SpritesPanelImpl

}

#endif // _DRAG2D_SPRITES_PANEL_IMPL_H_