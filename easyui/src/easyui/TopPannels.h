#ifndef _EASYUI_TOP_PANNELS_H_
#define _EASYUI_TOP_PANNELS_H_

namespace ee { class PropertySettingPanel; class ViewlistPanel; }

namespace eui
{

class TopLibraryPanel;
class TopStagePanel;
class TopToolbarPanel;

class TopPannels
{
public:
	TopLibraryPanel* library;

	ee::PropertySettingPanel* property;

	TopStagePanel* stage;

	ee::ViewlistPanel* viewlist;
	TopToolbarPanel* toolbar;

public:
	TopPannels();

}; // TopPannels

}

#endif // _EASYUI_TOP_PANNELS_H_
