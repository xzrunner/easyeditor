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

	void LoadFromFile(const Json::Value& value, const std::string& dir);
	void StoreToFile(Json::Value& value, const std::string& dir) const;

	void InitFromLayers(const std::vector<Layer*>& layers);
	void LoadSymbolFromLayer();

	void SetViewlist(d2d::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}
	void SetStagePanel(StagePanel* stage) {
		m_stage = stage;
	}
	void InitPages(StagePanel* stage, d2d::PropertySettingPanel* property);

	void RefreshViewList();

private:
	d2d::ViewlistPanel* m_viewlist;

	StagePanel* m_stage;

}; // LibraryPanel

}

#endif // _LR_LIBRARY_PANEL_H_