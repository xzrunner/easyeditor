#ifndef _EASYUI_WRAPPER_EDIT_OP_H_
#define _EASYUI_WRAPPER_EDIT_OP_H_

#include <drag2d.h>

#include "EditClipboxOP.h"

namespace eui
{
namespace wrapper
{

class StagePanel;

class EditOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>, public EditClipboxOP
{
public:
	EditOP(StagePanel* stage, d2d::PropertySettingPanel* property);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnKeyUp(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

private:
	static const int NODE_RADIUS = 10;

private:
	StagePanel* m_stage;

	// 1 2
	// 0 3
	int m_clipbox_selected;

}; // EditOP

}
}

#endif // _EASYUI_WRAPPER_EDIT_OP_H_