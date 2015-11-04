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
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	//
	//	wxWindows interface
	//
	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL);

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiShapesImpl interface
	//
	virtual bool InsertShape(d2d::IShape* shape);
	virtual bool ClearAllShapes();
	virtual void TraverseShapes(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;

	Symbol* GetSymbol() { return m_symbol; }

	void LoadFromShadow();

private:
	Symbol* m_symbol;

	d2d::IShape* m_loop;

}; // StagePanel

}

#endif // _EASYSHADOW_STAGE_PANEL_H_