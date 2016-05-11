#ifndef _EASYANIM_STAGE_PANEL_H_
#define _EASYANIM_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/Visitor.h>
#include <ee/StageDropTarget.h>

namespace eanim
{

class KeyFrame;
class SkeletonData;

class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

	//
	// ee::EditPanel interface
	//
	virtual bool Update(float dt);

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL,
		bool order = true) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void OnMenuAddJointNode(wxCommandEvent& event);
	void OnMenuDelJointNode(wxCommandEvent& event);

	void Reorder(ee::Sprite* spr, bool up);
	void ReorderMost(ee::Sprite* spr, bool up);
	void Insert(ee::Sprite* spr);
	void Remove(ee::Sprite* spr);

	void InsertWithUD(ee::Sprite* spr);
	void InsertWithoutUD(ee::Sprite* spr);

	bool IsCurrFrameValid() const { return m_frame != NULL; }

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

private:
	class CheckUpdateVisitor : public ee::Visitor
	{
	public:
		CheckUpdateVisitor(int version);

		virtual void Visit(ee::Object* object, bool& next);

		bool NeedUpdate() const { return m_update; }

	private:
		int m_version;
		bool m_update;

	}; // CheckUpdateVisitor

	class StageDropTarget : public ee::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

	private:
		StagePanel* m_stage;

	}; // StageDropTarget

private:
	int m_layer_idx, m_frame_idx;
	KeyFrame* m_frame;

	bool m_refresh;

}; // StagePanel

}

#endif // _EASYANIM_STAGE_PANEL_H_