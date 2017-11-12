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
	virtual bool UpdateStage() override;

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::RefVisitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL,
		bool order = true) const override;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void OnMenuAddJointNode(wxCommandEvent& event);
	void OnMenuDelJointNode(wxCommandEvent& event);

	void Reorder(const ee::SprPtr& spr, bool up);
	void ReorderMost(const ee::SprPtr& spr, bool up);
	void Insert(const ee::SprPtr& spr, int idx);
	void Remove(const ee::SprPtr& spr);

	void InsertWithUD(const ee::SprPtr& spr, int idx);
	void InsertWithoutUD(const ee::SprPtr& spr, int idx);

	bool IsCurrFrameValid() const { return m_frame != NULL; }

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

private:
	class CheckUpdateVisitor : public ee::RefVisitor<ee::Sprite>
	{
	public:
		CheckUpdateVisitor();

		virtual void Visit(const ee::SprPtr& spr, bool& next) override;

		bool NeedUpdate() const { return m_update; }

	private:
		bool m_update;

	}; // CheckUpdateVisitor

	class StageDropTarget : public ee::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);

		virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) override;
		virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

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