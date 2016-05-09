#ifndef _EASYMODELING_STAGE_PANEL_H_
#define _EASYMODELING_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/Visitor.h>

namespace emodeling
{

class Body;
class Joint;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~StagePanel();

	virtual ee::Sprite* QuerySpriteByPos(const sm::vec2& pos) const;
	virtual void QuerySpritesByRect(const ee::Rect& rect, std::vector<ee::Sprite*>& result) const;		

	Joint* queryJointByPos(const sm::vec2& pos) const;
	void queryJointsByRect(const ee::Rect& rect, std::vector<Joint*>& result) const;

	void insertJoint(Joint* joint) {
		m_joints.push_back(joint);
	}
	void removeJoint(Joint* joint);

	void traverseBodies(ee::Visitor& visitor) const;
	void traverseJoints(ee::Visitor& visitor) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	static void loadBody(const std::string& filepath, Body& body);
	static void loadBody(ee::Sprite* sprite, Body& body);

	void Insert(ee::Sprite* spr);
	void Remove(ee::Sprite* spr);
	void Clear();

private:
	class PointQueryVisitor : public ee::Visitor
	{
	public:
		PointQueryVisitor(const sm::vec2& pos, ee::Sprite** pResult);
		virtual void Visit(ee::Object* object, bool& next);

	private:
		const sm::vec2& m_pos;
		ee::Sprite** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public ee::Visitor
	{
	public:
		RectQueryVisitor(const ee::Rect& rect, std::vector<ee::Sprite*>& result);
		virtual void Visit(ee::Object* object, bool& next);

	private:
		const ee::Rect& m_rect;
		std::vector<ee::Sprite*>& m_result;

	}; // RectQueryVisitor

private:
	std::vector<Body*> m_bodies;
	std::vector<Joint*> m_joints;

}; // StagePanel

}

#endif // _EASYMODELING_STAGE_PANEL_H_
