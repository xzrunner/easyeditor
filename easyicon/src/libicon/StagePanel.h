#ifndef _EASYICON_STAGE_PANEL_H_
#define _EASYICON_STAGE_PANEL_H_

#include <drag2d.h>

namespace eicon
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

}; // StagePanel

}

#endif // _EASYICON_STAGE_PANEL_H_
