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
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
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

	void Reorder(d2d::ISprite* spr, bool up);
	void ReorderMost(d2d::ISprite* spr, bool up);
	void Insert(d2d::ISprite* spr);
	void Remove(d2d::ISprite* spr);

	void InsertWithUD(d2d::ISprite* spr);
	void InsertWithoutUD(d2d::ISprite* spr);

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

private:
	class CheckUpdateVisitor : public d2d::IVisitor
	{
	public:
		CheckUpdateVisitor(int version);

		virtual void Visit(d2d::Object* object, bool& bFetchNext);

		bool NeedUpdate() const { return m_update; }

	private:
		int m_version;
		bool m_update;

	}; // CheckUpdateVisitor

}; // StagePanel

}

#endif // _EASYANIM_STAGE_PANEL_H_