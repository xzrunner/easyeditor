#ifndef _EASYUI_WINDOW_STAGE_PANEL_H_
#define _EASYUI_WINDOW_STAGE_PANEL_H_

#include "SymbolsCfg.h"
#include "UIStagePage.h"

#include <ee/MultiSpritesImpl.h>
#include <ee/CrossGuides.h>

namespace eui
{

class TopPannels;

namespace window
{

class Symbol;
class SymbolsCfg;
class ToolbarPanel;

class StagePanel : public UIStagePage, public ee::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage() override;

	//
	// UIStagePage interface
	//
	virtual void LoadFromFile(const char* filename) override;
	virtual void StoreToFile(const char* filename) const override;
	virtual void EnablePage(bool enable) override;
	virtual void OnPreview() override;
	virtual void OnCode() const override;

	//
	// MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::RefVisitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const override;

	void InitConfig();

	auto& GetSymbol() { return m_sym; }

	void SetViewSize(int width, int height);

	ee::CrossGuides& GetCrossGuides() { return m_guides; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InitSubjects();

private:
	std::shared_ptr<Symbol> m_sym;

	TopPannels* m_top_pannels;

	SymbolsCfg m_symbols_cfg;

	int m_toolbar_idx;
	ToolbarPanel* m_toolbar;

	ee::CrossGuides m_guides;

}; // StagePanel

}
}

#endif // _EASYUI_WINDOW_STAGE_PANEL_H_
