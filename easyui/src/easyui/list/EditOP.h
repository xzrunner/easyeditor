#ifndef _EASYUI_LIST_EDIT_OP_H_
#define _EASYUI_LIST_EDIT_OP_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class StagePanel;

class EditOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
{
public:
	EditOP(StagePanel* stage, d2d::PropertySettingPanel* property);

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

#endif // _EASYUI_LIST_EDIT_OP_H_