#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>
#include <ee/StageDropTarget.h>
#include <ee/Sprite.h>

namespace emesh
{

class Symbol;
class Shape;

class StagePanel : public ee::EditPanel, public ee::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);
// 	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
// 		ee::LibraryPanel* library, Sprite* sprite);
	virtual ~StagePanel();

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const;

	void SetSymbol(Symbol* symbol);
	const Symbol* GetSymbol() const;

 	Shape* GetShape();
// 	Sprite* getSprite() { return m_sprite; }

	// for background
	void SetBackground(ee::Sprite* bg) { 
		bg->Retain();
		m_background = bg; 
	}
	const ee::Sprite* GetBackground() const { return m_background; }
	void TranslateBackground(const ee::Vector& offset) { 
		if (m_background) {
			m_background->Translate(offset);
		}
	}

	void LoadFromSymbol(const ee::Symbol* symbol);

	void UpdateSymbol();

	void CreateShape();

protected:
	//
	//	interface MultiShapesImpl
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Init(ee::LibraryPanel* library);

	void InitSubjects();

private:
	class StageDropTarget : public ee::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);

		virtual bool OnDropSymbol(ee::Symbol* symbol, const ee::Vector& pos);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget 

private:
	ee::Sprite* m_background;

	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_