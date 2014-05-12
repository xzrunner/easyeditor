
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
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::SpritesPanelImpl interface
		//
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual d2d::ISprite* querySpriteByPos(const d2d::Vector& pos) const;
		virtual void querySpritesByRect(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result) const;		

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

	private:
		class PointQueryVisitor : public d2d::IVisitor
		{
		public:
			PointQueryVisitor(const d2d::Vector& pos, d2d::ISprite** pResult);
			virtual void visit(d2d::Object* object, bool& bFetchNext);

		private:
			const d2d::Vector& m_pos;
			d2d::ISprite** m_pResult;

		}; // PointQueryVisitor

		class RectQueryVisitor : public d2d::IVisitor
		{
		public:
			RectQueryVisitor(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result);
			virtual void visit(d2d::Object* object, bool& bFetchNext);

		private:
			const d2d::Rect& m_rect;
			std::vector<d2d::ISprite*>& m_result;

		}; // RectQueryVisitor

	private:
		std::vector<libmodeling::Body*> m_bodies;
		std::vector<libmodeling::Joint*> m_joints;

	}; // StagePanel
}

#endif // EMODELING_STAGE_PANEL_H
