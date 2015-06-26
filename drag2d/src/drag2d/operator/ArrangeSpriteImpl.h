#ifndef _DRAG2D_ARRANGE_SPRITE_IMPL_H_
#define _DRAG2D_ARRANGE_SPRITE_IMPL_H_

#include <wx/menu.h>

#include "ArrangeSpriteConfig.h"
#include "AutoAlign.h"
#include "SpriteCtrlNode.h"

#include "common/typedef.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class EditPanel;
class MultiSpritesImpl;
class PropertySettingPanel;
class IArrangeSpriteState;
class Camera;

class ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
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
	ISprite* QueryEditedSprite(const Vector& pos) const;

	bool IsSelectionEmpty() const;

protected:
	virtual void OnDirectionKeyDown(DirectionType type);
	virtual void OnSpaceKeyDown();

	virtual void SetRightPopupMenu(wxMenu& menu, ISprite* spr);

	virtual IArrangeSpriteState* CreateTransalteState(SpriteSelection* selection, const Vector& first_pos) const;
	virtual IArrangeSpriteState* CreateRotateState(SpriteSelection* selection, const Vector& first_pos) const;
	virtual IArrangeSpriteState* CreateScaleState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;
	virtual IArrangeSpriteState* CreateShearState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;
	virtual IArrangeSpriteState* CreateOffsetState(ISprite* sprite) const;
	virtual IArrangeSpriteState* CreatePerspectiveState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const;

protected:
	SpriteSelection* m_selection;

private:
	void OnDeleteKeyDown();

	void UpOneLayer();
	void DownOneLayer();

	void UpLayerMost();
	void DownLayerMost();

	void HoriMirror();
	void VertMirror();

	void SetSelectionPerspBound();
	void SetSelectionOriginBound();

private:
	static const float CTRL_NODE_RADIUS;
	static const float MAX_CTRL_NODE_RADIUS;

private:
	enum
	{
		MENU_UP_ONE_LAYER = 10000,
		MENU_DOWN_ONE_LAYER,
		MENU_UP_MOST,
		MENU_DOWN_MOST,
		MENU_HORI_MIRROR,
		MENU_VERT_MIRROR,

		MENU_INSERT_TO_DTEX,
		MENU_REMOVE_FROM_DTEX
	};

private:
	const ArrangeSpriteConfig m_cfg;

	AutoAlign m_align;

	EditPanel* m_stage;
	MultiSpritesImpl* m_sprites_impl;
	PropertySettingPanel* m_property_panel;

	IArrangeSpriteState* m_op_state;

	Vector m_left_down_pos, m_right_down_pos;

	mutable float m_ctrl_node_radius;

}; // ArrangeSpriteImpl

}

#endif // _DRAG2D_ARRANGE_SPRITE_IMPL_H_