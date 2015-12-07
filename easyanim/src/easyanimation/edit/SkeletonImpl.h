#ifndef _EASYANIM_SKELETON_IMPL_H_
#define _EASYANIM_SKELETON_IMPL_H_

#include <drag2d.h>

namespace eanim
{

class Joint;

class SkeletonImpl : public d2d::ArrangeSpriteImpl
{
public:
	SkeletonImpl();

	virtual void OnKeyDown(int keyCode);
	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseDrag(int x, int y);

	virtual void OnPopMenuSelected(int type);

	virtual void OnDraw() const;

protected:
	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

	virtual d2d::IArrangeSpriteState* CreateTranslateState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;
	virtual d2d::IArrangeSpriteState* CreateRotateState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;

private:
	Joint* m_selected_joint;

	d2d::Vector m_first_pos;

}; // SkeletonImpl 

}

#endif // _EASYANIM_SKELETON_IMPL_H_