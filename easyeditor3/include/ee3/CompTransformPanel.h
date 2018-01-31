#pragma once

#include <ee0/NodeCompPanel.h>

#include <node3/CompTransform.h>

class wxTextCtrl;

namespace ee0 { class SubjectMgr; }

namespace ee3
{

class CompTransformPanel : public ee0::NodeCompPanel
{
public:
	CompTransformPanel(wxWindow* parent, n3::CompTransform& trans,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateTextValue(wxCommandEvent& event);

private:
	n3::CompTransform& m_trans;
	ee0::SubjectMgr& m_sub_mgr;

	wxTextCtrl *m_pos_x, *m_pos_y, *m_pos_z;
	wxTextCtrl *m_angle_x, *m_angle_y, *m_angle_z;
	wxTextCtrl *m_scale_x, *m_scale_y, *m_scale_z;

}; // CompTransformPanel

}