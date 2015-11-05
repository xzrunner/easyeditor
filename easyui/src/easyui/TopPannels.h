#ifndef _EASYUI_TOP_PANNELS_H_
#define _EASYUI_TOP_PANNELS_H_

#include <drag2d.h>

namespace eui
{

class TopLibraryPanel;
class TopStagePanel;
class TopToolbarPanel;

class TopPannels
{
public:
	TopLibraryPanel* library;

	d2d::PropertySettingPanel* property;

	TopStagePanel* stage;

	TopToolbarPanel* toolbar;

public:
	TopPannels();

}; // TopPannels

}

#endif // _EASYUI_TOP_PANNELS_H_
