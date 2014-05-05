#ifndef _ESHAPE_LIBRARY_PANEL_H_
#define _ESHAPE_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace eshape
{

class StagePanel;
class ToolbarPanel;
class LibraryShapePage;
class LibraryImagePage;

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

	virtual void onPageChanged(wxBookCtrlEvent& event);

	d2d::ILibraryPage* getShapePage();
	d2d::ILibraryPage* getImagePage();

	void setContext(StagePanel* stage, ToolbarPanel* toolbar);

private:
	LibraryShapePage* m_shapePage;
	LibraryImagePage* m_imagePage;

	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // LibraryPanel

}

#endif // _ESHAPE_LIBRARY_PANEL_H_