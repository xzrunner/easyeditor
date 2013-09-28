#pragma once

#include <drag2d.h>

namespace libcomplex
{
	class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
			d2d::ComplexSymbol* symbol);

		d2d::ComplexSymbol* getSymbol() { return m_symbol; }

	private:
		d2d::ComplexSymbol* m_symbol;

	}; // StagePanel
}

