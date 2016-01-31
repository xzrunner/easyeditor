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
		ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* spritesImpl, 
		ee::EditCMPT* callback = NULL);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void SetPropertyPanel(ee::PropertySettingPanel* property) { 
		m_property_panel = property; 
	}

private:
	class DrawSelectedVisitor : public ee::Visitor
	{
	public:
		virtual void Visit(ee::Object* object, bool& next);
	}; // DrawSelectedVisitor

private:
	ee::PropertySettingPanel* m_property_panel;

	libmodeling::Joint* m_selected;
	libmodeling::Joint* m_mouseOn;

	ee::Vector m_first_pos;

public:
	ee::SelectionSet<libmodeling::Joint> jointSelection;

//		std::vector<Joint*> selectedJoints;

}; // SelectJointOP

}

#endif // EMODELING_SELECT_JOINT_OP_H
