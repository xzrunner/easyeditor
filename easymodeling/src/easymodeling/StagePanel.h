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

	virtual d2d::Sprite* QuerySpriteByPos(const d2d::Vector& pos) const;
	virtual void QuerySpritesByRect(const d2d::Rect& rect, std::vector<d2d::Sprite*>& result) const;		

	libmodeling::Joint* queryJointByPos(const d2d::Vector& pos) const;
	void queryJointsByRect(const d2d::Rect& rect, std::vector<libmodeling::Joint*>& result) const;

	void insertJoint(libmodeling::Joint* joint) {
		m_joints.push_back(joint);
	}
	void removeJoint(libmodeling::Joint* joint);

	void traverseBodies(d2d::Visitor& visitor) const;
	void traverseJoints(d2d::Visitor& visitor) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	static void loadBody(const wxString& filepath, libmodeling::Body& body);
	static void loadBody(d2d::Sprite* sprite, libmodeling::Body& body);

	void Insert(d2d::Sprite* spr);
	void Remove(d2d::Sprite* spr);
	void Clear();

private:
	class PointQueryVisitor : public d2d::Visitor
	{
	public:
		PointQueryVisitor(const d2d::Vector& pos, d2d::Sprite** pResult);
		virtual void Visit(d2d::Object* object, bool& next);

	private:
		const d2d::Vector& m_pos;
		d2d::Sprite** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public d2d::Visitor
	{
	public:
		RectQueryVisitor(const d2d::Rect& rect, std::vector<d2d::Sprite*>& result);
		virtual void Visit(d2d::Object* object, bool& next);

	private:
		const d2d::Rect& m_rect;
		std::vector<d2d::Sprite*>& m_result;

	}; // RectQueryVisitor

private:
	std::vector<libmodeling::Body*> m_bodies;
	std::vector<libmodeling::Joint*> m_joints;

}; // StagePanel

}

#endif // EMODELING_STAGE_PANEL_H
