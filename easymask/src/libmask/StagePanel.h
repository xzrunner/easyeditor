#ifndef _EASYMASK_STAGE_PANEL_H_
#define _EASYMASK_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>

namespace ee { class PropertySettingPanel; }

namespace emask
{

class Symbol;
class StageCanvas;
class LibraryPanel;

class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, LibraryPanel* library);
	virtual ~StagePanel();

	//
	// ee::EditPanel interface
	//
	virtual bool Update(float dt);

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const {}

	Symbol* GetSymbol() { return m_sym; }
	void SetSymbol(Symbol* symbol);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	LibraryPanel* m_library;

	Symbol* m_sym;

}; // StagePanel

}

#endif // _EASYMASK_STAGE_PANEL_H_