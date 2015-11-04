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
	// d2d::EditPanel interface
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

	void SetSymbol(Symbol* symbol);
	const Symbol* GetSymbol() const;

 	Shape* GetShape();
// 	Sprite* getSprite() { return m_sprite; }

	// for background
	void SetBackground(d2d::ISprite* bg) { 
		bg->Retain();
		m_background = bg; 
	}
	const d2d::ISprite* GetBackground() const { return m_background; }
	void TranslateBackground(const d2d::Vector& offset) { 
		if (m_background) {
			m_background->Translate(offset);
		}
	}

	void LoadFromSymbol(const d2d::ISymbol* symbol);

	void UpdateSymbol();

	void CreateShape();

private:
	void init(d2d::LibraryPanel* library);

private:
	class StageDropTarget : public d2d::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library);

		virtual bool OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget 

private:
	d2d::ISprite* m_background;

	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_