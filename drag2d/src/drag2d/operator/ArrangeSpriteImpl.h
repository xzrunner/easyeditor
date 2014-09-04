#ifndef _DRAG2D_ARRANGE_SPRITE_IMPL_H_
#define _DRAG2D_ARRANGE_SPRITE_IMPL_H_

#include <wx/menu.h>

#include "AutoAlign.h"
#include "SpriteCtrlNode.h"

#include "common/typedef.h"
#include "dataset/SelectionSet.h"

namespace d2d
{

class MultiSpritesImpl;
class PropertySettingPanel;
class IArrangeSpriteState;

class ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel, bool isDeformOpen = true,
		bool isAutoAlignOpen = true, bool isOffsetOpen = true);
	~ArrangeSpriteImpl();

	virtual void onKeyDown(int keyCode);
	virtual void onKeyUp(int keyCode);
	virtual void onMouseLeftDown(int x, int y);
	virtual void onMouseLeftUp(int x, int y);
	virtual void onMouseRightDown(int x, int y);
	virtual void onMouseRightUp(int x, int y);
	virtual void onMouseDrag(int x, int y);

	virtual void onPopMenuSelected(int type);

	virtual void onDraw(const Screen& scr) const;
	virtual void clear();

	// query edited sprite, include its extra edit region
	ISprite* QueryEditedSprite(const Vector& pos) const;

protected:
	virtual void onDirectionKeyDown(DirectionType type);
	virtual void onSpaceKeyDown();

	virtual void setRightPopupMenu(wxMenu& menu);

	virtual IArrangeSpriteState* CreateTransalteState(SpriteSelection* selection, const Vector& first_pos) const;
	virtual IArrangeSpriteState* CreateRotateState(SpriteSelection* selection, const Vector& first_pos) const;
	virtual IArrangeSpriteState* CreateScaleState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;
	virtual IArrangeSpriteState* CreateShearState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;
	virtual IArrangeSpriteState* CreateOffsetState(ISprite* sprite) const;

protected:
	SpriteSelection* m_selection;

private:
	void UpOneLayer();
	void DownOneLayer();

private:
	static const float SCALE_NODE_RADIUS;

private:
	AutoAlign m_align;

	EditPanel* m_editPanel;
	MultiSpritesImpl* m_spritesImpl;
	PropertySettingPanel* m_propertyPanel;

	bool m_isDeformOpen;

	IArrangeSpriteState* m_op_state;

	bool m_is_offset_open;

	Vector m_right_down_pos;

}; // ArrangeSpriteImpl

}

#endif // _DRAG2D_ARRANGE_SPRITE_IMPL_H_