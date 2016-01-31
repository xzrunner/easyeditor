#ifndef _EASYSHADOW_STAGE_PANEL_H_
#define _EASYSHADOW_STAGE_PANEL_H_



namespace eshadow
{

class Symbol;

class StagePanel : public ee::EditPanel, public ee::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	//
	//	wxWindows interface
	//
	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL);

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const;

	Symbol* GetSymbol() { return m_symbol; }

	void LoadFromShadow();

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InsertShape(ee::Shape* shape);

private:
	Symbol* m_symbol;

	ee::Shape* m_loop;

}; // StagePanel

}

#endif // _EASYSHADOW_STAGE_PANEL_H_