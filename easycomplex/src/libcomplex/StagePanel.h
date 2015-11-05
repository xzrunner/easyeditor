#ifndef _DRAG2D_STAGE_PANEL_H_
#define _DRAG2D_STAGE_PANEL_H_

#include <drag2d.h>

namespace ecomplex
{

class LibraryPanel;
class Symbol;
class ArrangeSpriteImpl;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::PropertySettingPanel* property, LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, Symbol* symbol, 
		d2d::PropertySettingPanel* property, LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual bool Update(int version);

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	Symbol* getSymbol() { return m_symbol; }

private:
	void Clear();

private:
	Symbol* m_symbol;

	LibraryPanel* m_library;

}; // StagePanel

}

#endif // _DRAG2D_STAGE_PANEL_H_