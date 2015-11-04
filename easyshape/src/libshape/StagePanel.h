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
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		Symbol* symbol/*, d2d::LibraryPanel* library*/);
	virtual ~StagePanel();

	//
	// EditPanel interface
	//
	virtual void Clear();

	//
	//	interface MultiShapesImpl
	//
	virtual void Notify(int sj_id, void* ud);

	//
	// d2d::MultiShapesImpl interface
	//
	virtual bool InsertShape(d2d::IShape* shape);
	virtual bool ClearAllShapes();
	virtual void TraverseShapes(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;

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