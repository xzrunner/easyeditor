#ifndef _EASYSCALE9_RESIZE_BASE_OP_H_
#define _EASYSCALE9_RESIZE_BASE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

namespace escale9
{

class Symbol;

class ResizeBaseOP : public ee::ZoomViewOP
{
public:
	ResizeBaseOP(wxWindow* wnd, ee::EditPanelImpl* stage, Symbol* symbol);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

	void setSymbol(Symbol* symbol) const {
		if (!m_symbol) m_symbol = symbol;
	}

protected:
	enum Status
	{
		e_null = 0,
		e_leftlow,
		e_rightlow,
		e_leftup,
		e_rightup
	};

	static const int REGION = 10;

protected:
	Status m_status;

private:
	mutable Symbol* m_symbol;

	sm::vec2 m_firstPos;

}; // ResizeBaseOP

}

#endif // _EASYSCALE9_RESIZE_BASE_OP_H_