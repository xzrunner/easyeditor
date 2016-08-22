#ifndef _EASYSCALE9_STAGE_PANEL_H_
#define _EASYSCALE9_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>

namespace ee { class LibraryPanel; }

namespace escale9
{

class Symbol;
class ToolbarPanel;

class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::Visitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL,
		bool order = true) const;

	Symbol* getPatchSymbol() { return m_sym; }

 	ee::Sprite* getSprite(int row, int col);

	void rebuildPatchSymbol();

	void setToolbarPanel(ToolbarPanel* toolbar);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Insert(ee::Sprite* spr);
	void Remove(ee::Sprite* spr);
	void Clear();

private:
	// 6 7 8
	// 3 4 5
	// 0 1 2
	s2::Sprite* m_sprs[9];

	Symbol* m_sym;

	ee::LibraryPanel* m_library;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _EASYSCALE9_STAGE_PANEL_H_