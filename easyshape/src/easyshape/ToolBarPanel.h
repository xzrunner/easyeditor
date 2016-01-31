#ifndef _ESHAPE_TOOLBAR_PANEL_H_
#define _ESHAPE_TOOLBAR_PANEL_H_



namespace eshape
{

class StagePanel;
class LibraryItem;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::PropertySettingPanel* property,
		StagePanel* stage);

	void changeCurrItem(LibraryItem* item);

protected:
	virtual wxSizer* initLayout();

private:
	void onClearShapes(wxCommandEvent& event);

}; // ToolbarPanel

}

#endif // _ESHAPE_TOOLBAR_PANEL_H_