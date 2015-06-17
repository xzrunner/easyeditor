#ifndef _EASYANIM_WIDGETS_H_
#define _EASYANIM_WIDGETS_H_

#include <drag2d.h>

namespace eanim
{

class StagePanel;
class KeysPanel;
class LayersPanel;
class ToolbarPanel;

class Widgets
{
public:
	Widgets();
	
public:
	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;
	LayersPanel* m_layersPanel;
	KeysPanel* m_keysPanel;
	d2d::ViewlistPanel* m_viewlist;

	d2d::ILibraryPage* m_imagePage;

	d2d::ViewPanelMgr m_view_panel_mgr;

}; // Widgets

}

#endif // _EASYANIM_WIDGETS_H_