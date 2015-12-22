#ifndef _EASYUI_WRAPPER_STAGE_PANEL_H_
#define _EASYUI_WRAPPER_STAGE_PANEL_H_

#include <drag2d.h>

#include "UIStagePage.h"

namespace eui
{

class TopPannels;

namespace wrapper
{

class ToolbarPanel;

class StagePanel : public UIStagePage, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, TopPannels* top_pannels);

	//
	// UIStagePage interface
	//
	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;
	virtual void EnablePage(bool enable);
	virtual void OnPreview() {}
	virtual void OnCode() const {}

	d2d::Rect& GetClipbox() { return m_clipbox; }

private:
	TopPannels* m_top_pannels;
	ToolbarPanel* m_toolbar;

	int m_toolbar_idx;

	d2d::Rect m_clipbox;

}; // StagePanel

}
}

#endif // _EASYUI_WRAPPER_STAGE_PANEL_H_