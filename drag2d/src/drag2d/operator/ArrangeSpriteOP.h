#pragma once

#include "interfaces.h"

namespace d2d
{

class ArrangeSpriteImpl;
class EditPanel;
class MultiSpritesImpl;
class PropertySettingPanel;
class ViewPanelMgr;
class AbstractEditCMPT;
struct ArrangeSpriteConfig;

template <typename TBase>
class ArrangeSpriteOP : public TBase
{
public:
	ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel = NULL, ViewPanelMgr* view_panel_mgr = NULL,
		AbstractEditCMPT* callback = NULL, const ArrangeSpriteConfig& cfg = ArrangeSpriteConfig(), 
		ArrangeSpriteImpl* impl = NULL);
	virtual ~ArrangeSpriteOP();

	virtual bool onKeyDown(int keyCode);
	virtual bool onKeyUp(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onPopMenuSelected(int type);

	virtual bool onDraw() const;
	virtual bool clear();

	virtual bool IsEmpty() const;

protected:
	virtual ISprite* selectByPos(const Vector& pos) const;

private:
	ArrangeSpriteImpl* m_impl;
	
}; // ArrangeSpriteOP

}

#include "ArrangeSpriteOP.cpp"

