#ifndef _DRAG2D_ARRANGE_SPRITE_OP_H_
#define _DRAG2D_ARRANGE_SPRITE_OP_H_

#include "interfaces.h"

#include <wx/wx.h>

namespace d2d
{

class ArrangeSpriteImpl;
class EditPanelImpl;
class MultiSpritesImpl;
class PropertySettingPanel;
class AbstractEditCMPT;
struct ArrangeSpriteConfig;

template <typename TBase>
class ArrangeSpriteOP : public TBase
{
public:
	ArrangeSpriteOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* spritesImpl, 
		PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL, 
		const ArrangeSpriteConfig& cfg = ArrangeSpriteConfig(), 
		ArrangeSpriteImpl* impl = NULL);
	virtual ~ArrangeSpriteOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnKeyUp(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnPopMenuSelected(int type);

	virtual bool OnDraw() const;
	virtual bool Clear();

	virtual bool IsEmpty() const;

protected:
	virtual ISprite* SelectByPos(const Vector& pos) const;

private:
	ArrangeSpriteImpl* m_impl;
	
}; // ArrangeSpriteOP

}

#include "ArrangeSpriteOP.cpp"

#endif // _DRAG2D_ARRANGE_SPRITE_OP_H_