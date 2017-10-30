#ifndef _EASYSCALE9_RESIZE_BASE_OP_H_
#define _EASYSCALE9_RESIZE_BASE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

#include <memory>

namespace escale9
{

class Symbol;

class ResizeBaseOP : public ee::ZoomViewOP
{
public:
	ResizeBaseOP(wxWindow* wnd, ee::EditPanelImpl* stage, const std::shared_ptr<Symbol>& sym);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;

	void setSymbol(const std::shared_ptr<Symbol>& sym) const {
		m_sym = sym;
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
	mutable std::shared_ptr<Symbol> m_sym;

	sm::vec2 m_firstPos;

}; // ResizeBaseOP

}

#endif // _EASYSCALE9_RESIZE_BASE_OP_H_