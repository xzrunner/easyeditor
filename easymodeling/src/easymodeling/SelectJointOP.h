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
	SelectJointOP(wxWindow* stage_wnd,
		d2d::EditPanelImpl* stage, 
		d2d::MultiSpritesImpl* spritesImpl, 
		d2d::ViewPanelMgr* view_panel_mgr = NULL,
		d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void SetPropertyPanel(d2d::PropertySettingPanel* property) { 
		m_property_panel = property; 
	}

private:
	class DrawSelectedVisitor : public d2d::IVisitor
	{
	public:
		virtual void Visit(d2d::Object* object, bool& bFetchNext);
	}; // DrawSelectedVisitor

private:
	d2d::PropertySettingPanel* m_property_panel;

	libmodeling::Joint* m_selected;
	libmodeling::Joint* m_mouseOn;

	d2d::Vector m_first_pos;

public:
	d2d::SelectionSet<libmodeling::Joint> jointSelection;

//		std::vector<Joint*> selectedJoints;

}; // SelectJointOP

}

#endif // EMODELING_SELECT_JOINT_OP_H
