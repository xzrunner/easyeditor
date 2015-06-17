
#ifndef EMODELING_SELECT_JOINT_OP_H
#define EMODELING_SELECT_JOINT_OP_H

#include "SelectBodyOP.h"

namespace libmodeling { class Joint; }

namespace emodeling
{
	class StagePanel;

	class SelectJointOP : public SelectBodyOP
	{
	public:
		SelectJointOP(d2d::EditPanel* editPanel, 
			d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, 
			d2d::ViewPanelMgr* view_panel_mgr = NULL,
			d2d::AbstractEditCMPT* callback = NULL);

		virtual bool OnKeyDown(int keyCode);
		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseLeftUp(int x, int y);
		virtual bool OnMouseMove(int x, int y);
		virtual bool OnMouseDrag(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

	private:
		class DrawSelectedVisitor : public d2d::IVisitor
		{
		public:
			virtual void visit(d2d::Object* object, bool& bFetchNext);
		}; // DrawSelectedVisitor

	private:
		libmodeling::Joint* m_selected;
		libmodeling::Joint* m_mouseOn;

		d2d::Vector m_firstPos;

	public:
		d2d::SelectionSet<libmodeling::Joint> jointSelection;

//		std::vector<Joint*> selectedJoints;

	}; // SelectJointOP
}

#endif // EMODELING_SELECT_JOINT_OP_H
