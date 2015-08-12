#ifndef _EASYSCALE9_STAGE_PANEL_H_
#define _EASYSCALE9_STAGE_PANEL_H_

#include <drag2d.h>

namespace escale9
{

class Symbol;
class ToolbarPanel;

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	virtual bool RemoveSprite(d2d::ISprite* sprite);
	virtual bool ClearAllSprite();
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL,
		bool order = true) const;

	Symbol* getPatchSymbol() { return m_symbol; }

	d2d::ISprite* getSprite(int row, int col) {
		if (row < 0 || row >= 3 || col < 0 || col >= 3)
			return NULL;
		return m_sprites[row][col];
	}

	void rebuildPatchSymbol();

	void setToolbarPanel(ToolbarPanel* toolbar);

private:
	// [2][0]
	// [1][0]
	// [0][0] [0][1] [0][2]
	d2d::ISprite* m_sprites[3][3];

	Symbol* m_symbol;

	d2d::LibraryPanel* m_library;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _EASYSCALE9_STAGE_PANEL_H_