#ifndef _EASYMESH_STAGE_PANEL_H_
#define _EASYMESH_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>
#include <ee/StageDropTarget.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>

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
		const ee::SprPtr& edited, const ee::MultiSpritesImpl* bg_sprites);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::RefVisitor<ee::Shape>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const;

	void SetMeshSymbol(const std::shared_ptr<Symbol>& sym);
	const Symbol& GetMeshSymbol() const;

 	Mesh* GetMesh();
// 	Sprite* getSprite() { return m_spr; }

	// for background
	void SetBackground(const ee::SprPtr& bg) { 
		m_background = bg; 
	}
	const ee::SprPtr& GetBackground() const { return m_background; }
	void TranslateBackground(const sm::vec2& offset) { 
		if (m_background) {
			m_background->Translate(offset);
		}
	}

	void LoadFromSymbol(const ee::SymConstPtr& sym);

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

		virtual bool OnDropSymbol(const ee::SymPtr& sym, const sm::vec2& pos);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget 

private:
	ee::SprPtr m_background;

	std::shared_ptr<Symbol> m_sym;

}; // StagePanel

}

#endif // _EASYMESH_STAGE_PANEL_H_