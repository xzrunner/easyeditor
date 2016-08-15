#ifndef _EASYSHAPE_STAGE_PANEL_H_
#define _EASYSHAPE_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>
#include <ee/CombinedDropTarget.h>
 
namespace ee { class LibraryPanel; class Sprite; class MultiSpritesImpl; class Symbol; }

class wxGLContext;

namespace eshape
{

class Symbol;
class ToolbarPanel;

class StagePanel : public ee::EditPanel, public ee::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* bg_sprites);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		Symbol* symbol/*, ee::LibraryPanel* library*/);
	virtual ~StagePanel();

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::Visitor<ee::Shape>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const;

	void LoadFromFile(const char* filename);
	void StoreToFile(const char* filename) const;

	//virtual void loadShapes();
	//virtual void storeShapes() const;

	const ee::Symbol& GetSymbol() const;

	void SetToolbarPanel(ToolbarPanel* toolbar) {
		m_toolbar = toolbar;
	}

	void SetSymbolBG(ee::Symbol* symbol);

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitSubjects();

private:
	class DropTarget : public ee::CombinedDropTarget
	{
	public:
		DropTarget(StagePanel* stage, ee::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& data);
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

	private:
		StagePanel* m_stage;
		ee::LibraryPanel* m_library;

	}; // DragSymbolTarget

private:
	eshape::Symbol* m_symbol;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _EASYSHAPE_STAGE_PANEL_H_