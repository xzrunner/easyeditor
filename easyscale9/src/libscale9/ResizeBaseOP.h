#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class ResizeBaseOP : public d2d::ZoomViewOP
	{
	public:
		ResizeBaseOP(d2d::EditPanel* editPanel, Symbol* symbol);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;

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

