#pragma once

#include <vector>
#include <wx/wx.h>

#include "interfaces.h"
#include "interfaces.h"

namespace d2d
{
	class Symbol;
	class LibraryPanel;

	class PasteSymbolRandomWidget : public wxPanel
	{
	public:
		struct RandomValue
		{
			Symbol* symbol;
			float scale;
			float angle;	// in radian
		};

	public:
		PasteSymbolRandomWidget(wxWindow* parent, LibraryPanel* libraryPanel);

		void getRandomValue(RandomValue& val) const;

	private:
		void initLayout();

	private:
		class FilterSymbolVisitor : public Visitor
		{
		public:
			FilterSymbolVisitor(const wxString& filter, std::vector<Symbol*>& result);

			virtual void Visit(Object* object, bool& next);

		private:
			wxString m_filter;

			std::vector<Symbol*>& m_result;
		};

	private:
		LibraryPanel* m_libraryPanel;

		wxTextCtrl* m_symbolFilterCtrl;
		wxSpinCtrl *m_scaleMinCtrl, *m_scaleMaxCtrl;
		wxSpinCtrl *m_angleMinCtrl, *m_angleMaxCtrl;

	}; // PasteSymbolRandomWidget
}

