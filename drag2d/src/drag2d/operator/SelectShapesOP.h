#ifndef _D2D_SELECT_SHAPES_OP_H_
#define _D2D_SELECT_SHAPES_OP_H_

#include "DrawRectangleOP.h"
#include "interfaces.h"

namespace d2d
{

class MultiShapesImpl;
class AbstractEditCMPT;
class IPropertySetting;
class Shape;
class ShapeSelection;

class SelectShapesOP : public DrawRectangleOP
{
public:
	SelectShapesOP(wxWindow* wnd, EditPanelImpl* stage, MultiShapesImpl* shapesImpl, 
		AbstractEditCMPT* callback = NULL);
	virtual ~SelectShapesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	void clearClipboard();

	void PasteToSelection() const;
	void CopyFromSelection();

protected:
	Vector m_first_pos;

	ShapeSelection* m_selection;

private:
	class TranslateVisitor : public Visitor
	{
	public:
		TranslateVisitor(const Vector& offset) : m_offset(offset) {}
		virtual void Visit(Object* object, bool& next);
	private:
		Vector m_offset;
	}; // TranslateVisitor

private:
	AbstractEditCMPT* m_callback;

	MultiShapesImpl* m_shapeImpl;

	mutable std::vector<Shape*> m_clipboard;

	// To disable mouse able when press ctrl and window query
	bool m_bDraggable;

	Vector m_move_last_pos;

}; // SelectShapesOP

}

#endif // _D2D_SELECT_SHAPES_OP_H_