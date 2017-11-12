#ifndef _EASYUI_LIST_STAGE_PANEL_H_
#define _EASYUI_LIST_STAGE_PANEL_H_

#include "UIList.h"
#include "UIStagePage.h"

#include <ee/MultiSpritesImpl.h>

class wxGLContext;

namespace eui
{

class TopPannels;

namespace list
{

class ToolbarPanel;

class StagePanel : public UIStagePage, public ee::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
		TopPannels* top_pannels);

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::RefVisitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const override;

	//
	// UIStagePage interface
	//
	virtual void LoadFromFile(const char* filename) override;
	virtual void StoreToFile(const char* filename) const override;
	virtual void EnablePage(bool enable) override;
	virtual void OnPreview() override {}
	virtual void OnCode() const override {}

	UIList& GetList() { return m_list; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	TopPannels* m_top_pannels;
	ToolbarPanel* m_toolbar;

	UIList m_list;

	int m_toolbar_idx;

}; // StagePanel

}
}

#endif // _EASYUI_LIST_STAGE_PANEL_H_