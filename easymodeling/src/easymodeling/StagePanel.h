#ifndef EMODELING_STAGE_PANEL_H
#define EMODELING_STAGE_PANEL_H

#include <drag2d.h>

namespace libmodeling
{
	class Body;
	class Joint;
}

namespace emodeling
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~StagePanel();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	virtual d2d::ISprite* QuerySpriteByPos(const d2d::Vector& pos) const;
	virtual void QuerySpritesByRect(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result) const;		

	libmodeling::Joint* queryJointByPos(const d2d::Vector& pos) const;
	void queryJointsByRect(const d2d::Rect& rect, std::vector<libmodeling::Joint*>& result) const;

	void insertJoint(libmodeling::Joint* joint) {
		m_joints.push_back(joint);
	}
	void removeJoint(libmodeling::Joint* joint);

	void traverseBodies(d2d::IVisitor& visitor) const;
	void traverseJoints(d2d::IVisitor& visitor) const;

private:
	static void loadBody(const wxString& filepath, libmodeling::Body& body);
	static void loadBody(d2d::ISprite* sprite, libmodeling::Body& body);

	void Insert(d2d::ISprite* spr);
	void Remove(d2d::ISprite* spr);
	void Clear();

private:
	class PointQueryVisitor : public d2d::IVisitor
	{
	public:
		PointQueryVisitor(const d2d::Vector& pos, d2d::ISprite** pResult);
		virtual void Visit(d2d::Object* object, bool& bFetchNext);

	private:
		const d2d::Vector& m_pos;
		d2d::ISprite** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public d2d::IVisitor
	{
	public:
		RectQueryVisitor(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result);
		virtual void Visit(d2d::Object* object, bool& bFetchNext);

	private:
		const d2d::Rect& m_rect;
		std::vector<d2d::ISprite*>& m_result;

	}; // RectQueryVisitor

private:
	std::vector<libmodeling::Body*> m_bodies;
	std::vector<libmodeling::Joint*> m_joints;

	std::vector<d2d::Subject*> m_subjects;

}; // StagePanel

}

#endif // EMODELING_STAGE_PANEL_H
