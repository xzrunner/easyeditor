#ifndef _EASYSHAPE_STAGE_PANEL_H_
#define _EASYSHAPE_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>
#include <ee/CombinedDropTarget.h>
#include <ee/Symbol.h>

namespace ee { class LibraryPanel; class Sprite; class MultiSpritesImpl; }

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
		wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* bg_sprites);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		const std::shared_ptr<Symbol>& sym/*, ee::LibraryPanel* library*/);

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::RefVisitor<ee::Shape>& visitor,
		ee::DataTraverseType type = ee::DT_ALL) const override;

	void LoadFromFile(const char* filename);
	void StoreToFile(const char* filename) const;

	//virtual void loadShapes();
	//virtual void storeShapes() const;

	const ee::Symbol& GetSymbol() const;

	void SetToolbarPanel(ToolbarPanel* toolbar) {
		m_toolbar = toolbar;
	}

	void SetSymbolBG(const ee::SymPtr& sym);

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InitSubjects();

private:
	class DropTarget : public ee::CombinedDropTarget
	{
	public:
		DropTarget(StagePanel* stage, ee::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& data) override;
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

	private:
		StagePanel* m_stage;
		ee::LibraryPanel* m_library;

	}; // DragSymbolTarget

private:
	std::shared_ptr<Symbol> m_sym;

	ToolbarPanel* m_toolbar;

}; // StagePanel

}

#endif // _EASYSHAPE_STAGE_PANEL_H_