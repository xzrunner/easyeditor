#ifndef _EASYEDITOR_SPRITES_PANEL_IMPL_H_
#define _EASYEDITOR_SPRITES_PANEL_IMPL_H_

#include "MultiSpritesImpl.h"

namespace ee
{

class EditPanelImpl;
class LibraryPanel;
class Sprite;
class DataContainer;

class SpritesPanelImpl : public MultiSpritesImpl
{
public:
	SpritesPanelImpl(EditPanelImpl* stage, DataContainer* container);
	SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library);
	virtual ~SpritesPanelImpl();

	//
	// MultiSpritesImpl interface
	//
	virtual void TraverseSprites(Visitor& visitor, 
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

	DataContainer* m_container;

}; // SpritesPanelImpl

}

#endif // _EASYEDITOR_SPRITES_PANEL_IMPL_H_