#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>
#include <ee/StageDropTarget.h>
#include <ee/Sprite.h>

class wxGLContext;

namespace ee { class MultiSpritesImpl; }

namespace emesh
{

class Symbol;
class Mesh;

class StagePanel : public ee::EditPanel, public ee::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx, 
		ee::LibraryPanel* library = NULL);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx, 
		ee::Sprite* edited, const ee::MultiSpritesImpl* bg_sprites);
	virtual ~StagePanel();

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::Visitor<ee::Shape>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const;

	void SetMeshSymbol(Symbol* sym);
	const Symbol* GetMeshSymbol() const;

 	Mesh* GetMesh();
// 	Sprite* getSprite() { return m_spr; }

	// for background
	void SetBackground(ee::Sprite* bg) { 
		bg->AddReference();
		m_background = bg; 
	}
	const ee::Sprite* GetBackground() const { return m_background; }
	void TranslateBackground(const sm::vec2& offset) { 
		if (m_background) {
			m_background->Translate(offset);
		}
	}

	void LoadFromSymbol(const ee::Symbol* sym);

	void UpdateSymbol();

	void RecreateMesh();

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

		virtual bool OnDropSymbol(ee::Symbol* sym, const sm::vec2& pos);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget 

private:
	ee::Sprite* m_background;

	Symbol* m_sym;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_