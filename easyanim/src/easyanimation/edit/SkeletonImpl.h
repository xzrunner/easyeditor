#ifndef _EASYANIM_SKELETON_IMPL_H_
#define _EASYANIM_SKELETON_IMPL_H_

#include <drag2d.h>

#include "KeyDownHandler.h"

namespace eanim
{

class StagePanel;
class Joint;

class SkeletonImpl : public d2d::ArrangeSpriteImpl
{
public:
	SkeletonImpl(StagePanel* stage, d2d::PropertySettingPanel* property,
		d2d::AbstractEditOP* editop);

	virtual void onKeyDown(int keyCode);
	virtual void onMouseLeftDown(int x, int y);
	virtual void onMouseLeftUp(int x, int y);
	virtual void onMouseRightDown(int x, int y);
	virtual void onMouseDrag(int x, int y);

	virtual void onPopMenuSelected(int type);

	virtual void onDraw() const;

protected:
	virtual void setRightPopupMenu(wxMenu& menu);

	virtual d2d::IArrangeSpriteState* CreateTransalteState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;
	virtual d2d::IArrangeSpriteState* CreateRotateState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;

private:
	StagePanel* m_stage;

	KeyDownHandler m_key_handler;

	Joint* m_selected_joint;

	d2d::Vector m_first_pos;

}; // SkeletonImpl 

}

#endif // _EASYANIM_SKELETON_IMPL_H_