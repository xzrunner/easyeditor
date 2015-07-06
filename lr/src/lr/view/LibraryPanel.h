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

	virtual void OnPageChanged(wxBookCtrlEvent& event);
	virtual void OnPageChanging(wxBookCtrlEvent& event);

	void LoadFromFile(const Json::Value& value, const std::string& dir);
	void StoreToFile(Json::Value& value, const std::string& dir) const;

	void InitFromLayers(const std::vector<Layer*>& layers);
	void LoadSymbolFromLayer();

	void SetViewlist(d2d::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}
	void SetGroupTree(d2d::GroupTreePanel* grouptree) {
		m_grouptree = grouptree;
	}
	void SetStagePanel(StagePanel* stage) {
		m_stage = stage;
	}
	void InitPages(StagePanel* stage, d2d::PropertySettingPanel* property,
		d2d::ViewPanelMgr* view_panel_mgr);

	void Refresh();

private:
	void CharHook(wxKeyEvent& event);

private:
	d2d::ViewlistPanel* m_viewlist;
	d2d::GroupTreePanel* m_grouptree;

	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // LibraryPanel

}

#endif // _LR_LIBRARY_PANEL_H_