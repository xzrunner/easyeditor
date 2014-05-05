#ifndef ESHAPE_TOOLBAR_PANEL_H
#define ESHAPE_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace eshape
{

class StagePanel;
class LibraryItem;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent);

	void changeCurrItem(LibraryItem* item);

protected:
	virtual wxSizer* initLayout();

private:
	void onClearShapes(wxCommandEvent& event);

}; // ToolbarPanel

}

#endif // ESHAPE_TOOLBAR_PANEL_H