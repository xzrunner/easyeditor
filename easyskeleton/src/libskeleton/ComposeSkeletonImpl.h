#ifndef _EASYSKELETON_COMPOSE_SKELETON_IMPL_H_
#define _EASYSKELETON_COMPOSE_SKELETON_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>

namespace ee { class PropertySettingPanel; }

namespace eskeleton
{

class StagePanel;

class ComposeSkeletonImpl : public ee::ArrangeSpriteImpl
{
public:
	ComposeSkeletonImpl(StagePanel* stage, ee::PropertySettingPanel* property);

	virtual bool OnKeyDown(int keyCode);
	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseDrag(int x, int y);

	virtual void OnPopMenuSelected(int type);

	virtual void OnDraw(const ee::Camera& cam) const;

protected:
	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

// 	virtual ee::ArrangeSpriteState* CreateTranslateState(ee::SpriteSelection* selection, 
// 		const sm::vec2& first_pos) const;
// 	virtual ee::ArrangeSpriteState* CreateRotateState(ee::SpriteSelection* selection, 
// 		const sm::vec2& first_pos) const;

private:
//	Joint* m_selected_joint;

	sm::vec2 m_first_pos;

}; // ComposeSkeletonImpl

}

#endif // _EASYSKELETON_COMPOSE_SKELETON_IMPL_H_