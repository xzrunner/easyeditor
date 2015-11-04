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
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	//
	// MultiSpritesImpl interface
	//
	virtual void TraverseSprites(IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const;

private:
	EditPanelImpl* m_stage;

	IDataContainer* m_container;

	std::vector<Subject*> m_subjects;

}; // SpritesPanelImpl

}

#endif // _DRAG2D_SPRITES_PANEL_IMPL_H_