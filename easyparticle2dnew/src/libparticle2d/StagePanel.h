#ifndef _EASYPARTICLE2D_STAGEPANEL_H_
#define _EASYPARTICLE2D_STAGEPANEL_H_

#include <drag2d.h>

#include "StageData.h"

namespace eparticle2d
{

class LibraryPanel;
class ToolbarPanel;
class Symbol;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);

	void SetSelectedSymbol(Symbol* symbol);
	void SetToolbar(ToolbarPanel* toolbar);

	StageData* GetStageData() { return &m_data; }

private:
	StageData m_data;

}; // StagePanel

}

#endif // _EASYPARTICLE2D_STAGEPANEL_H_
