#ifndef _EASYEDITOR_PASTE_SYMBOL_RANDOM_WIDGET_H_
#define _EASYEDITOR_PASTE_SYMBOL_RANDOM_WIDGET_H_

#include "Visitor.h"

#include <wx/wx.h>

#include <vector>

namespace ee
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

	void GetRandomValue(RandomValue& val) const;

private:
	void InitLayout();

private:
	class FilterSymbolVisitor : public Visitor
	{
	public:
		FilterSymbolVisitor(const std::string& filter, std::vector<Symbol*>& result);

		virtual void Visit(Object* object, bool& next);

	private:
		std::string m_filter;

		std::vector<Symbol*>& m_result;
	};

private:
	LibraryPanel* m_library;

	wxTextCtrl* m_sym_filter_ctrl;
	wxSpinCtrl *m_scale_min_ctrl, *m_scale_max_ctrl;
	wxSpinCtrl *m_angle_min_ctrl, *m_angle_max_ctrl;

}; // PasteSymbolRandomWidget

}

#endif // _EASYEDITOR_PASTE_SYMBOL_RANDOM_WIDGET_H_