#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <drag2d.h>

namespace emesh
{

class Sprite;
class Shape;
class StagePanel : public d2d::EditPanel, public d2d::MultiShapesImpl
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

	//
	// d2d::MultiShapesImpl interface
	//
	virtual void traverseShapes(d2d::IVisitor& visitor, 
		d2d::TraverseType type = d2d::e_allExisting) const;
	virtual void removeShape(d2d::IShape* shape);
	virtual void insertShape(d2d::IShape* shape);
	virtual void clearShapes();

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
	class StageDropTarget : public d2d::StageDropTarget
	{
	public:
		StageDropTarget(d2d::Frame* frame, StagePanel* stage, 
			d2d::LibraryPanel* library);

		virtual bool OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget 

private:
	Sprite* m_sprite;

	d2d::ISprite* m_background;

	std::vector<d2d::IShape*> m_shapes;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_