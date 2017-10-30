#ifndef _EASYEDITOR_PASTE_SYMBOL_RANDOM_WIDGET_H_
#define _EASYEDITOR_PASTE_SYMBOL_RANDOM_WIDGET_H_

#include "Visitor.h"
#include "ListItem.h"
#include "Symbol.h"

#include <wx/wx.h>

#include <vector>

namespace ee
{

class LibraryPanel;

class PasteSymbolRandomWidget : public wxPanel
{
public:
	struct RandomValue
	{
		SymPtr sym;
		float scale;
		float angle;	// in radian
	};

public:
	PasteSymbolRandomWidget(wxWindow* parent, LibraryPanel* library);

	void GetRandomValue(RandomValue& val) const;

private:
	void InitLayout();

private:
	class FilterSymbolVisitor : public RefVisitor<ListItem>
	{
	public:
		FilterSymbolVisitor(const std::string& filter);

		virtual void Visit(const ListItemPtr& item, bool& next);

		auto& GetResult() const { return m_result; }

	private:
		std::string m_filter;

		std::vector<SymPtr> m_result;
	};

private:
	LibraryPanel* m_library;

	wxTextCtrl* m_sym_filter_ctrl;
	wxSpinCtrl *m_scale_min_ctrl, *m_scale_max_ctrl;
	wxSpinCtrl *m_angle_min_ctrl, *m_angle_max_ctrl;

}; // PasteSymbolRandomWidget

}

#endif // _EASYEDITOR_PASTE_SYMBOL_RANDOM_WIDGET_H_