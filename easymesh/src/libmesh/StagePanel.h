#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <drag2d.h>

namespace emesh
{

class Sprite;
class Shape;
class StagePanel : public d2d::EditPanel
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library, Sprite* sprite);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	Shape* getShape();
	Sprite* getSprite() { return m_sprite; }

	// for background
	void SetBackground(d2d::ISprite* bg) { 
		bg->retain();
		m_background = bg; 
	}
	const d2d::ISprite* GetBackground() const { return m_background; }
	void TranslateBackground(const d2d::Vector& offset) { 
		if (m_background) {
			m_background->translate(offset);
		}
	}

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

	d2d::ISprite* m_background;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_