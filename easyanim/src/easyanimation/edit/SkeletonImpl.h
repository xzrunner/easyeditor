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
		LayersMgr* layers);

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
	StagePanel* m_stage;

	KeyDownHandler m_key_handler;

	Joint* m_selected_joint;

	d2d::Vector m_first_pos;

}; // SkeletonImpl 

}

#endif // _EASYANIM_SKELETON_IMPL_H_