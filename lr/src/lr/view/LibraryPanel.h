#ifndef _LR_LIBRARY_PANEL_H_
#define _LR_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace lr
{

class Layer;
class StagePanel;

class LibraryPanel : public d2d::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

	virtual void onPageChanged(wxBookCtrlEvent& event);

	void InitFromLayers(const std::vector<Layer*>& layers);

	void SetViewlist(d2d::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}
	void InitPages(StagePanel* stage, d2d::PropertySettingPanel* property);

	void RefreshViewList();

private:
	d2d::ViewlistPanel* m_viewlist;

}; // LibraryPanel

}

#endif // _LR_LIBRARY_PANEL_H_