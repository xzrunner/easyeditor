#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <drag2d.h>

namespace emesh
{

class Symbol;
class Shape;

class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
// 	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
// 		d2d::LibraryPanel* library, Sprite* sprite);
	virtual ~StagePanel();

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void TraverseShapes(d2d::Visitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL) const;

	void SetSymbol(Symbol* symbol);
	const Symbol* GetSymbol() const;

 	Shape* GetShape();
// 	Sprite* getSprite() { return m_sprite; }

	// for background
	void SetBackground(d2d::Sprite* bg) { 
		bg->Retain();
		m_background = bg; 
	}
	const d2d::Sprite* GetBackground() const { return m_background; }
	void TranslateBackground(const d2d::Vector& offset) { 
		if (m_background) {
			m_background->Translate(offset);
		}
	}

	void LoadFromSymbol(const d2d::Symbol* symbol);

	void UpdateSymbol();

	void CreateShape();

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Init(d2d::LibraryPanel* library);

	void InitSubjects();

private:
	class StageDropTarget : public d2d::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library);

		virtual bool OnDropSymbol(d2d::Symbol* symbol, const d2d::Vector& pos);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget 

private:
	d2d::Sprite* m_background;

	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_