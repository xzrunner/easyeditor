#ifndef _ESHAPE_TOOLBAR_PANEL_H_
#define _ESHAPE_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eshape
{

class StagePanel;
class LibraryItem;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::PropertySettingPanel* property,
		StagePanel* stage);

	void changeCurrItem(LibraryItem* item);

protected:
	virtual wxSizer* initLayout();

private:
	void onClearShapes(wxCommandEvent& event);

}; // ToolbarPanel

}

#endif // _ESHAPE_TOOLBAR_PANEL_H_