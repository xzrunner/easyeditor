#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <drag2d.h>

namespace emesh
{

class Sprite;
class Mesh;
class StagePanel : public d2d::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library, Sprite* sprite);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	Mesh* getMesh();
	Sprite* getSprite() { return m_sprite; }

private:
	void init(d2d::LibraryPanel* library);

private:
	class DragSymbolTarget : public wxTextDropTarget
	{
	public:
		DragSymbolTarget(StagePanel* stage, d2d::LibraryPanel* library) 
			: m_stage(stage), m_library(library) {}

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		StagePanel* m_stage;
		d2d::LibraryPanel* m_library;

	}; // DragSymbolTarget

private:
	Sprite* m_sprite;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_