#ifndef _EASYEDITOR_ARRANGE_SPRITE_OP_H_
#define _EASYEDITOR_ARRANGE_SPRITE_OP_H_

#include "Sprite.h"

#include <SM_Vector.h>

#include <wx/wx.h>

namespace ee
{

class ArrangeSpriteImpl;
class EditPanelImpl;
class MultiSpritesImpl;
class PropertySettingPanel;
class EditCMPT;
struct ArrangeSpriteConfig;

template <typename TBase>
class ArrangeSpriteOP : public TBase
{
public:
	ArrangeSpriteOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl, 
		PropertySettingPanel* property = nullptr, EditCMPT* callback = nullptr,
		const ArrangeSpriteConfig& cfg = ArrangeSpriteConfig(), 
		ArrangeSpriteImpl* impl = nullptr);
	virtual ~ArrangeSpriteOP();

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnKeyUp(int keyCode) override;
	virtual bool OnChar(int keyCode) override;

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnPopMenuSelected(int type);

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	virtual bool IsEmpty() const override;

protected:
	virtual SprPtr SelectByPos(const sm::vec2& pos) const;

private:
	ArrangeSpriteImpl* m_impl;
	
}; // ArrangeSpriteOP

}

#include "ArrangeSpriteOP.inl"

#endif // _EASYEDITOR_ARRANGE_SPRITE_OP_H_