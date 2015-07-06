
#ifndef EMODELING_PREVIEW_PANEL_H
#define EMODELING_PREVIEW_PANEL_H

#include <Box2D/Box2D.h>
#include <drag2d.h>
#include <easyphysics.h>

namespace libmodeling
{
	class Body;
	class Joint;
}

namespace emodeling
{
	class PreviewPanel : public d2d::EditPanel, public ephysics::PhysicsPanelImpl
	{
	public:
		PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame);

	private:
		void createGround();

	private:
		class LoadBodyVisitor : public d2d::IVisitor
		{
		public:
			LoadBodyVisitor(b2World* world, std::map<libmodeling::Body*, b2Body*>& mapBody);
			virtual void Visit(d2d::Object* object, bool& bFetchNext);

		private:
			b2World* m_world;

			std::map<libmodeling::Body*, b2Body*>& m_mapBody;

		}; // LoadBodyVisitor

		class LoadJointVisitor : public d2d::IVisitor
		{
		public:
			LoadJointVisitor(b2World* world, const std::map<libmodeling::Body*, b2Body*>& mapBody,
				std::map<libmodeling::Joint*, b2Joint*>& mapJoint);

			virtual void Visit(d2d::Object* object, bool& bFetchNext);

		private:
			b2World* m_world;

			const std::map<libmodeling::Body*, b2Body*>& m_mapBody;

			std::map<libmodeling::Joint*, b2Joint*>& m_mapJoint;

		}; // LoadJointVisitor

		class LoadGearJointVisitor : public d2d::IVisitor
		{
		public:
			LoadGearJointVisitor(b2World* world, const std::map<libmodeling::Body*, b2Body*>& mapBody,
				const std::map<libmodeling::Joint*, b2Joint*>& mapJoint);

			virtual void Visit(d2d::Object* object, bool& bFetchNext);

		private:
			b2World* m_world;

			const std::map<libmodeling::Body*, b2Body*>& m_mapBody;
			const std::map<libmodeling::Joint*, b2Joint*>& m_mapJoint;

		}; // LoadGearJointVisitor

	private:
		b2Body* m_ground;

	}; // PreviewPanel
}

#endif // EMODELING_PREVIEW_PANEL_H
