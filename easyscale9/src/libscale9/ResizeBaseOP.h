#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class ResizeBaseOP : public d2d::ZoomViewOP
	{
	public:
		ResizeBaseOP(wxWindow* wnd, d2d::EditPanelImpl* stage, Symbol* symbol);

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

		d2d::Vector m_firstPos;

		d2d::ShapeStyle m_style;

	}; // ResizeBaseOP
}

