#ifndef _EASYMODELING_SELECT_JOINT_OP_H_
#define _EASYMODELING_SELECT_JOINT_OP_H_

#include "SelectBodyOP.h"

#include <ee/SelectionSet.h>

namespace ee { class PropertySettingPanel; }

namespace emodeling
{

class Joint;
class StagePanel;

class SelectJointOP : public SelectBodyOP
{
public:
	SelectJointOP(wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, 
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

	Joint* m_selected;
	Joint* m_mouseOn;

	ee::Vector m_first_pos;

public:
	ee::SelectionSet<Joint> jointSelection;

//		std::vector<Joint*> selectedJoints;

}; // SelectJointOP

}

#endif // _EASYMODELING_SELECT_JOINT_OP_H_
