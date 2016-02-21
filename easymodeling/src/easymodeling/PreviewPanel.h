#ifndef _EASYMODELING_PREVIEW_PANEL_H_
#define _EASYMODELING_PREVIEW_PANEL_H_

#include <Box2D/Box2D.h>

#include <ee/EditPanel.h>
#include <ee/PhysicsPanelImpl.h>
#include <ee/Visitor.h>

#include <map>

namespace emodeling
{

class Body;
class Joint;

class PreviewPanel : public ee::EditPanel, public ee::PhysicsPanelImpl
{
public:
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame);

private:
	void createGround();

private:
	class LoadBodyVisitor : public ee::Visitor
	{
	public:
		LoadBodyVisitor(b2World* world, std::map<Body*, b2Body*>& mapBody);
		virtual void Visit(ee::Object* object, bool& next);

	private:
		b2World* m_world;

		std::map<Body*, b2Body*>& m_mapBody;

	}; // LoadBodyVisitor

	class LoadJointVisitor : public ee::Visitor
	{
	public:
		LoadJointVisitor(b2World* world, const std::map<Body*, b2Body*>& mapBody,
			std::map<Joint*, b2Joint*>& mapJoint);

		virtual void Visit(ee::Object* object, bool& next);

	private:
		b2World* m_world;

		const std::map<Body*, b2Body*>& m_mapBody;

		std::map<Joint*, b2Joint*>& m_mapJoint;

	}; // LoadJointVisitor

	class LoadGearJointVisitor : public ee::Visitor
	{
	public:
		LoadGearJointVisitor(b2World* world, const std::map<Body*, b2Body*>& mapBody,
			const std::map<Joint*, b2Joint*>& mapJoint);

		virtual void Visit(ee::Object* object, bool& next);

	private:
		b2World* m_world;

		const std::map<Body*, b2Body*>& m_mapBody;
		const std::map<Joint*, b2Joint*>& m_mapJoint;

	}; // LoadGearJointVisitor

private:
	b2Body* m_ground;

}; // PreviewPanel

}

#endif // _EASYMODELING_PREVIEW_PANEL_H_
