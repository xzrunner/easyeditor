#ifndef _EASYUI_WRAPPER_STAGE_PANEL_H_
#define _EASYUI_WRAPPER_STAGE_PANEL_H_

#include "UIStagePage.h"

#include <ee/SpritesPanelImpl.h>
#include <ee/Rect.h>

class wxGLContext;

namespace eui
{

class TopPannels;

namespace wrapper
{

class ToolbarPanel;

class StagePanel : public UIStagePage, public ee::SpritesPanelImpl
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

	ee::Rect& GetClipbox() { return m_clipbox; }

private:
	TopPannels* m_top_pannels;
	ToolbarPanel* m_toolbar;

	int m_toolbar_idx;

	ee::Rect m_clipbox;

}; // StagePanel

}
}

#endif // _EASYUI_WRAPPER_STAGE_PANEL_H_