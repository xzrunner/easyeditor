#ifndef _EASYEDITOR_SPRITES_PANEL_IMPL_H_
#define _EASYEDITOR_SPRITES_PANEL_IMPL_H_

#include "MultiSpritesImpl.h"
#include "DataContainer.h"

namespace ee
{

class EditPanelImpl;
class LibraryPanel;
class Sprite;

class SpritesPanelImpl : public MultiSpritesImpl
{
public:
	SpritesPanelImpl(EditPanelImpl* stage, DataContainer<Sprite>* container);
	SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library);
	virtual ~SpritesPanelImpl();

	//
	// MultiSpritesImpl interface
	//
	virtual void TraverseSprites(Visitor<Sprite>& visitor, 
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

	DataContainer<Sprite>* m_container;

}; // SpritesPanelImpl

}

#endif // _EASYEDITOR_SPRITES_PANEL_IMPL_H_