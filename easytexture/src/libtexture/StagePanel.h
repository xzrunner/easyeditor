#ifndef _EASYTEXTURE_STAGE_PANEL_H_
#define _EASYTEXTURE_STAGE_PANEL_H_

#include <drag2d.h>

namespace etexture
{

class Symbol;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
	public d2d::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		Symbol* symbol, d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

// 	Symbol* GetSymbol() { return m_symbol; }
// 
private:
	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYTEXTURE_STAGE_PANEL_H_
