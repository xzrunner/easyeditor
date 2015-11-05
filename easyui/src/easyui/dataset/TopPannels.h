#ifndef _EASYUI_TOP_PANNELS_H_
#define _EASYUI_TOP_PANNELS_H_

#include <drag2d.h>

namespace eui
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class TopPannels
{
public:
	LibraryPanel* library;

	d2d::PropertySettingPanel* property;

	StagePanel* stage;

	ToolbarPanel* toolbar;

public:
	TopPannels();

}; // TopPannels

}

#endif // _EASYUI_TOP_PANNELS_H_
