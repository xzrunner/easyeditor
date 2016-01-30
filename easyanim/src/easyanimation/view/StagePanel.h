#ifndef _EASYANIM_STAGE_PANEL_H_
#define _EASYANIM_STAGE_PANEL_H_

#include <drag2d.h>

namespace eanim
{

class KeyFrame;
class SkeletonData;

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);

	//
	// d2d::EditPanel interface
	//
	virtual bool Update(int version);

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(d2d::Visitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL,
		bool order = true) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void OnMenuAddJointNode(wxCommandEvent& event);
	void OnMenuDelJointNode(wxCommandEvent& event);

	void Reorder(d2d::Sprite* spr, bool up);
	void ReorderMost(d2d::Sprite* spr, bool up);
	void Insert(d2d::Sprite* spr);
	void Remove(d2d::Sprite* spr);

	void InsertWithUD(d2d::Sprite* spr);
	void InsertWithoutUD(d2d::Sprite* spr);

	bool IsCurrFrameValid() const { return m_frame != NULL; }

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

private:
	class CheckUpdateVisitor : public d2d::Visitor
	{
	public:
		CheckUpdateVisitor(int version);

		virtual void Visit(d2d::Object* object, bool& next);

		bool NeedUpdate() const { return m_update; }

	private:
		int m_version;
		bool m_update;

	}; // CheckUpdateVisitor

	class StageDropTarget : public d2d::StageDropTarget
	{
	public:
		StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library);

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