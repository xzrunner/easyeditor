#ifndef _EASYSHADOW_STAGE_PANEL_H_
#define _EASYSHADOW_STAGE_PANEL_H_

#include <drag2d.h>

namespace eshadow
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

#endif // _EASYSHADOW_STAGE_PANEL_H_