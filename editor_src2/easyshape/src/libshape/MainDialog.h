
#ifndef LIBSHAPE_MAIN_DIALOG_H
#define LIBSHAPE_MAIN_DIALOG_H

#include <drag2d.h>

namespace libshape
{
	class ToolbarPanel;

	template <typename TStage>
	class MainDialog : public wxDialog
	{
	public:
		MainDialog(wxWindow* parent, d2d::ISymbol* symbol);
		MainDialog(wxWindow* parent, d2d::ISprite* sprite);
		virtual ~MainDialog();

	private:
		void onSize(wxSizeEvent& event);

		void initLayout();

	private:
		d2d::ISymbol* m_symbol;
		d2d::ISprite* m_sprite;

		wxWindow* m_root;

		TStage* m_stage;
		ToolbarPanel* m_toolbar;

	}; // MainDialog
}

#include "MainDialog.cpp"

#endif // LIBSHAPE_MAIN_DIALOG_H