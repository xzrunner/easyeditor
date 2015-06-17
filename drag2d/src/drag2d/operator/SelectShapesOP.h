#ifndef _D2D_SELECT_SHAPES_OP_H_
#define _D2D_SELECT_SHAPES_OP_H_

#include "DrawRectangleOP.h"

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class EditPanel;
class MultiShapesImpl;
class AbstractEditCMPT;
class IPropertySetting;
class IShape;
class ViewPanelMgr;

class SelectShapesOP : public DrawRectangleOP
{
public:
	SelectShapesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
		ViewPanelMgr* view_panel_mgr = NULL, AbstractEditCMPT* callback = NULL);
	virtual ~SelectShapesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	void clearClipboard();

protected:
	Vector m_firstPos;

	ShapeSelection* m_selection;

private:
	AbstractEditCMPT* m_callback;

	MultiShapesImpl* m_shapeImpl;

	std::vector<IShape*> m_clipboard;

	// To disable mouse able when press ctrl and window query
	bool m_bDraggable;

	ViewPanelMgr* m_view_panel_mgr;

}; // SelectShapesOP

}

#endif // _D2D_SELECT_SHAPES_OP_H_