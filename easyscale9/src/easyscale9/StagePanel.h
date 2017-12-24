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
	virtual void TraverseSprites(ee::RefVisitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL,
		bool order = true) const override;

	std::shared_ptr<Symbol> getPatchSymbol() { return m_sym; }

 	ee::SprPtr getSprite(int row, int col);

	void rebuildPatchSymbol();

	void setToolbarPanel(ToolbarPanel* toolbar);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void Insert(const ee::SprPtr& spr);
	void Remove(const ee::SprPtr& spr);
	void Clear();

private:
	// 6 7 8
	// 3 4 5
	// 0 1 2
	s2::SprPtr m_sprs[9];

	std::shared_ptr<Symbol> m_sym;

	ee::LibraryPanel* m_library;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _EASYSCALE9_STAGE_PANEL_H_