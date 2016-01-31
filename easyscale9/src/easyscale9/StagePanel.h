#ifndef _EASYSCALE9_STAGE_PANEL_H_
#define _EASYSCALE9_STAGE_PANEL_H_



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
	virtual void TraverseSprites(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL,
		bool order = true) const;

	Symbol* getPatchSymbol() { return m_symbol; }

	ee::Sprite* getSprite(int row, int col) {
		if (row < 0 || row >= 3 || col < 0 || col >= 3)
			return NULL;
		return m_sprites[row][col];
	}

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
	// [2][0]
	// [1][0]
	// [0][0] [0][1] [0][2]
	ee::Sprite* m_sprites[3][3];

	Symbol* m_symbol;

	ee::LibraryPanel* m_library;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _EASYSCALE9_STAGE_PANEL_H_