#ifndef _EASYEDITOR_SPRITES_PANEL_IMPL_H_
#define _EASYEDITOR_SPRITES_PANEL_IMPL_H_

#include "MultiSpritesImpl.h"
#include "DataContainer.h"
#include "Sprite.h"

namespace ee
{

class EditPanelImpl;
class LibraryPanel;

class SpritesPanelImpl : public MultiSpritesImpl
{
public:
	SpritesPanelImpl(EditPanelImpl* stage);
	SpritesPanelImpl(EditPanelImpl* stage, LibraryPanel* library);

	//
	// MultiSpritesImpl interface
	//
	virtual void TraverseSprites(RefVisitor<Sprite>& visitor,
		DataTraverseType type = DT_ALL, bool order = true) const override;

	void SetContainer(const std::shared_ptr<DataContainer<Sprite>>& container) {
		m_container = container;
	}

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InitSubjects();

private:
	EditPanelImpl* m_stage;

	std::shared_ptr<DataContainer<Sprite>> m_container = nullptr;

}; // SpritesPanelImpl

}

#endif // _EASYEDITOR_SPRITES_PANEL_IMPL_H_