#ifndef _LR_LIBRARY_PANEL_H_
#define _LR_LIBRARY_PANEL_H_

#include "dataset/layer_type.h"

#include <ee/LibraryPanel.h>
#include <ee/CameraModes.h>

#include <json/json.h>

namespace ee { class ViewlistPanel; class GroupTreePanel; class PropertySettingPanel; }

namespace lr
{

class Layer;
class StagePanel;
class LibraryPage;

class LibraryPanel : public ee::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

	virtual void OnPageChanged(wxBookCtrlEvent& event) override;
	virtual void OnPageChanging(wxBookCtrlEvent& event) override;

	void LoadFromFile(const Json::Value& value, const CU_STR& dir);
	void StoreToFile(Json::Value& value, const CU_STR& dir) const;

	void InitFromLayers(const std::vector<Layer*>& layers);
	void LoadSymbolFromLayer();

	void SetViewlist(ee::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}
	void SetGroupTree(ee::GroupTreePanel* grouptree) {
		m_grouptree = grouptree;
	}
	void SetStagePanel(StagePanel* stage) {
		m_stage = stage;
	}
	void InitPages(StagePanel* stage, ee::PropertySettingPanel* property);

	void Refresh();

	Layer* GetTerrainLayer();
	Layer* GetCollisionLayer();

	LayerType GetLayerType(int idx) const;
	pt2::CameraMode GetLayerCameraMode(int idx) const;

	Layer* GetLayer(int idx);

	bool IsCurrUnitLayer();
	bool IsCurrLevelLayer();

	void GetAllPathName(std::vector<std::string>& names) const;

private:
	void CharHook(wxKeyEvent& event);

private:
	ee::ViewlistPanel* m_viewlist;
	ee::GroupTreePanel* m_grouptree;

	StagePanel* m_stage;

	LibraryPage* m_terrain_page;
	LibraryPage* m_unit_page;
	LibraryPage* m_path_page;
	LibraryPage* m_level_page;
	LibraryPage* m_collision_page;

	DECLARE_EVENT_TABLE()

}; // LibraryPanel

}

#endif // _LR_LIBRARY_PANEL_H_