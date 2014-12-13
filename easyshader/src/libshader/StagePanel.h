#ifndef _ESHADER_STAGE_PANEL_H_
#define _ESHADER_STAGE_PANEL_H_

#include <drag2d.h>

namespace eshader
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);

}; // StagePanel

}

#endif // _ESHADER_STAGE_PANEL_H_