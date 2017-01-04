#ifndef _EASYANIM_SKELETON_IMPL_H_
#define _EASYANIM_SKELETON_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>

namespace eanim
{

class Joint;

class SkeletonImpl : public ee::ArrangeSpriteImpl
{
public:
	SkeletonImpl();

	virtual bool OnKeyDown(int keyCode);
	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseDrag(int x, int y);

	virtual void OnPopMenuSelected(int type);

	virtual void OnDraw() const;

protected:
	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

	virtual ee::ArrangeSpriteState* CreateTranslateState(ee::SpriteSelection* selection, 
		const sm::vec2& first_pos) const;
	virtual ee::ArrangeSpriteState* CreateRotateState(ee::SpriteSelection* selection, 
		const sm::vec2& first_pos) const;

private:
	Joint* m_selected_joint;

	sm::vec2 m_first_pos;

}; // SkeletonImpl 

}

#endif // _EASYANIM_SKELETON_IMPL_H_