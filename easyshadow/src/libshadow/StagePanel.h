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
		d2d::ISprite* edited, const std::vector<d2d::ISprite*>& bg_sprites);
	virtual ~StagePanel();

	//
	//	wxWindows interface
	//
	virtual void Refresh(bool eraseBackground=true, const wxRect *rect=NULL);

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void traverseShapes(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;
	virtual void removeShape(d2d::IShape* shape);
	virtual void insertShape(d2d::IShape* shape);
	virtual void clearShapes();

	Symbol* GetSymbol() { return m_symbol; }

	void LoadFromShadow();

private:
	Symbol* m_symbol;

	d2d::IShape* m_loop;

}; // StagePanel

}

#endif // _EASYSHADOW_STAGE_PANEL_H_