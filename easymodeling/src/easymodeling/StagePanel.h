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

	virtual ee::SprPtr QuerySpriteByPos(const sm::vec2& pos) const;
	virtual void QuerySpritesByRect(const sm::rect& rect, std::vector<ee::SprPtr>& result) const;

	Joint* queryJointByPos(const sm::vec2& pos) const;
	void queryJointsByRect(const sm::rect& rect, std::vector<Joint*>& result) const;

	void insertJoint(Joint* joint) {
		m_joints.push_back(joint);
	}
	void removeJoint(Joint* joint);

	void traverseBodies(ee::RefVisitor<ee::Sprite>& visitor) const;
	void traverseJoints(ee::RefVisitor<ee::Sprite>& visitor) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	static void loadBody(const std::string& filepath, Body& body);
	static void loadBody(const ee::SprPtr& spr, Body& body);

	void Insert(const ee::SprPtr& spr);
	void Remove(const ee::SprPtr& spr);
	void Clear();

private:
	class PointQueryVisitor : public ee::Visitor<ee::Sprite>
	{
	public:
		PointQueryVisitor(const sm::vec2& pos, ee::SprPtr* pResult);
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;

	private:
		const sm::vec2& m_pos;
		ee::SprPtr* m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public ee::Visitor<ee::Sprite>
	{
	public:
		RectQueryVisitor(const sm::rect& rect, std::vector<ee::SprPtr>& result);
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;

	private:
		const sm::rect& m_rect;
		std::vector<ee::SprPtr>& m_result;

	}; // RectQueryVisitor

private:
	std::vector<Body*> m_bodies;
	std::vector<Joint*> m_joints;

}; // StagePanel

}

#endif // _EASYMODELING_STAGE_PANEL_H_
