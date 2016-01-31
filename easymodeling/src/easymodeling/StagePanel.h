#ifndef EMODELING_STAGE_PANEL_H
#define EMODELING_STAGE_PANEL_H



namespace libmodeling
{
	class Body;
	class Joint;
}

namespace emodeling
{

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~StagePanel();

	virtual ee::Sprite* QuerySpriteByPos(const ee::Vector& pos) const;
	virtual void QuerySpritesByRect(const ee::Rect& rect, std::vector<ee::Sprite*>& result) const;		

	libmodeling::Joint* queryJointByPos(const ee::Vector& pos) const;
	void queryJointsByRect(const ee::Rect& rect, std::vector<libmodeling::Joint*>& result) const;

	void insertJoint(libmodeling::Joint* joint) {
		m_joints.push_back(joint);
	}
	void removeJoint(libmodeling::Joint* joint);

	void traverseBodies(ee::Visitor& visitor) const;
	void traverseJoints(ee::Visitor& visitor) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	static void loadBody(const wxString& filepath, libmodeling::Body& body);
	static void loadBody(ee::Sprite* sprite, libmodeling::Body& body);

	void Insert(ee::Sprite* spr);
	void Remove(ee::Sprite* spr);
	void Clear();

private:
	class PointQueryVisitor : public ee::Visitor
	{
	public:
		PointQueryVisitor(const ee::Vector& pos, ee::Sprite** pResult);
		virtual void Visit(ee::Object* object, bool& next);

	private:
		const ee::Vector& m_pos;
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
	std::vector<libmodeling::Body*> m_bodies;
	std::vector<libmodeling::Joint*> m_joints;

}; // StagePanel

}

#endif // EMODELING_STAGE_PANEL_H
