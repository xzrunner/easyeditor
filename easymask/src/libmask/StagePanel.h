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

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::RefVisitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const {}

	const std::shared_ptr<Symbol>& GetSymbol() { return m_sym; }
	void SetSymbol(const std::shared_ptr<Symbol>& sym) { m_sym = sym; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	LibraryPanel* m_library;

	std::shared_ptr<Symbol> m_sym;

}; // StagePanel

}

#endif // _EASYMASK_STAGE_PANEL_H_