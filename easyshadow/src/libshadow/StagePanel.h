#ifndef _EASYSHADOW_STAGE_PANEL_H_
#define _EASYSHADOW_STAGE_PANEL_H_

#include <drag2d.h>

namespace eshadow
{

class Symbol;

class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, d2d::Sprite* edited, 
		const d2d::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	//
	//	wxWindows interface
	//
	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL);

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void TraverseShapes(d2d::Visitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;

	Symbol* GetSymbol() { return m_symbol; }

	void LoadFromShadow();

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InsertShape(d2d::Shape* shape);

private:
	Symbol* m_symbol;

	d2d::Shape* m_loop;

}; // StagePanel

}

#endif // _EASYSHADOW_STAGE_PANEL_H_