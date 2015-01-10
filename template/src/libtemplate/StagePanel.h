#ifndef _EASYTEMPLATE_STAGE_PANEL_H_
#define _EASYTEMPLATE_STAGE_PANEL_H_

#include <drag2d.h>

namespace etemplate
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
	public d2d::ShapesPanelImpl
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

#endif // _EASYTEMPLATE_STAGE_PANEL_H_