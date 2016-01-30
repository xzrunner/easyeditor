#ifndef _DRAG2D_ARRANGE_SPRITE_IMPL_H_
#define _DRAG2D_ARRANGE_SPRITE_IMPL_H_

#include <wx/menu.h>

#include "ArrangeSpriteConfig.h"
#include "AutoAlign.h"
#include "SpriteCtrlNode.h"
#include "RightPopupMenu.h"

#include "common/typedef.h"

namespace d2d
{

class EditPanelImpl;
class MultiSpritesImpl;
class PropertySettingPanel;
class IArrangeSpriteState;
class Camera;
class SpriteSelection;

class ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel, const ArrangeSpriteConfig& cfg = ArrangeSpriteConfig());
	~ArrangeSpriteImpl();

	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseRightUp(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseDrag(int x, int y);

	virtual void OnPopMenuSelected(int type);

	virtual void OnDraw(const Camera& cam) const;
	virtual void Clear();

	// query edited sprite, include its extra edit region
	Sprite* QueryEditedSprite(const Vector& pos) const;

	bool IsSelectionEmpty() const;

protected:
	virtual void OnDirectionKeyDown(DirectionType type);
	virtual void OnSpaceKeyDown();

	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

	virtual IArrangeSpriteState* CreateTranslateState(SpriteSelection* selection, const Vector& first_pos) const;
	virtual IArrangeSpriteState* CreateRotateState(SpriteSelection* selection, const Vector& first_pos) const;
	virtual IArrangeSpriteState* CreateScaleState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;
	virtual IArrangeSpriteState* CreateShearState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;
	virtual IArrangeSpriteState* CreateOffsetState(Sprite* sprite) const;
	virtual IArrangeSpriteState* CreatePerspectiveState(Sprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;

protected:
	SpriteSelection* m_selection;

private:
	void OnDeleteKeyDown();

	void UpOneLayer();
	void DownOneLayer();

	Vector GetSprOffset(const Sprite* spr) const;

	void ChangeOPState(IArrangeSpriteState* state);

	void OnSpriteShortcutKey(int keycode);

private:
	static const float CTRL_NODE_RADIUS;
	static const float MAX_CTRL_NODE_RADIUS;

protected:
	EditPanelImpl* m_stage;

private:
	wxWindow* m_wnd;

	const ArrangeSpriteConfig m_cfg;

	AutoAlign m_align;

	MultiSpritesImpl* m_sprites_impl;
	PropertySettingPanel* m_property_panel;

	IArrangeSpriteState* m_op_state;

	Vector m_left_down_pos, m_right_down_pos;

	mutable float m_ctrl_node_radius;

	RightPopupMenu m_popup;

}; // ArrangeSpriteImpl

}

#endif // _DRAG2D_ARRANGE_SPRITE_IMPL_H_