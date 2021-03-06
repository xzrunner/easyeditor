#ifndef _EASYUI_WRAPPER_EDIT_OP_H_
#define _EASYUI_WRAPPER_EDIT_OP_H_

#include "EditClipboxOP.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace eui
{
namespace wrapper
{

class StagePanel;

class EditOP : public ee::ArrangeSpriteOP<ee::SelectSpritesOP>, public EditClipboxOP
{
public:
	EditOP(StagePanel* stage, ee::PropertySettingPanel* property);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnKeyUp(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;

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