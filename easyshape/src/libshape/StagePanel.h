#ifndef _LIBSHAPE_STAGE_PANEL_H_
#define _LIBSHAPE_STAGE_PANEL_H_

#include <drag2d.h>

namespace libshape
{

class Symbol;
class ToolbarPanel;

class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		Symbol* symbol);
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

	void LoadFromFile(const char* filename);
	void StoreToFile(const char* filename) const;

	//virtual void loadShapes();
	//virtual void storeShapes() const;

	const d2d::ISymbol& GetSymbol() const;

	void SetToolbarPanel(ToolbarPanel* toolbar) {
		m_toolbar = toolbar;
	}

	void SetSymbolBG(d2d::ISymbol* symbol);

private:
	class DropTarget : public d2d::CombinedDropTarget
	{
	public:
		DropTarget(StagePanel* stage, d2d::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& data);
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

	private:
		StagePanel* m_stage;
		d2d::LibraryPanel* m_library;

	}; // DragSymbolTarget

private:
	libshape::Symbol* m_symbol;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _LIBSHAPE_STAGE_PANEL_H_