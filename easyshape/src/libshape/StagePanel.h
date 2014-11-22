#ifndef _LIBSHAPE_STAGE_PANEL_H_
#define _LIBSHAPE_STAGE_PANEL_H_

#include <drag2d.h>

namespace libshape
{

class Symbol;

class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		Symbol* symbol, d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// EditPanel interface
	//
	virtual void clear();

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void traverseShapes(d2d::IVisitor& visitor, 
		d2d::TraverseType type = d2d::e_allExisting) const;
	virtual void removeShape(d2d::IShape* shape);
	virtual void insertShape(d2d::IShape* shape);
	virtual void clearShapes();

	//virtual void loadShapes();
	//virtual void storeShapes() const;

	const d2d::ISymbol& GetSymbol() const;

private:
	void SetSymbolBG(d2d::ISymbol* symbol);

private:
	class DragSymbolTarget : public wxTextDropTarget
	{
	public:
		DragSymbolTarget(StagePanel* stage, d2d::LibraryPanel* library);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		StagePanel* m_stage;
		d2d::LibraryPanel* m_library;

	}; // DragSymbolTarget

private:
	libshape::Symbol* m_symbol;

}; // StagePanel

}

#endif // _LIBSHAPE_STAGE_PANEL_H_