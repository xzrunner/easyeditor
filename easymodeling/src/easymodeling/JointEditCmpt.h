#ifndef _EASYMODELING_JOINT_EDIT_CMPT_H_
#define _EASYMODELING_JOINT_EDIT_CMPT_H_

#include <ee/EditCMPT.h>

namespace ee { class PropertySettingPanel; }

namespace emodeling
{

class StagePanel;

class JointEditCmpt : public ee::EditCMPT
{
public:
	JointEditCmpt(wxWindow* parent, const std::string& name,
		StagePanel* editPanel, ee::PropertySettingPanel* property);

	virtual void UpdateControlValue();

protected:
	virtual wxSizer* InitLayout();

private:
	void onCreateJoint(wxCommandEvent& event);

	void onTypeChanged(wxCommandEvent& event);

private:
	class SelectWheelDialog : public wxDialog
	{
	public:
		SelectWheelDialog(wxWindow* parent, const std::string& title, 
			const std::string& body0, const std::string& body1);

		int getChoice() const {
			return m_wheelChoice->GetSelection();
		}

	private:
		void InitLayout(const std::string& body0, const std::string& body1);

	private:
		wxChoice* m_wheelChoice;

	}; // SelectWheelDialog

private:
	StagePanel* m_stage_panel;

	wxChoice* m_typeChoice;

	wxButton* m_btnOK;

}; // JointEditCmpt

}

#endif // _EASYMODELING_JOINT_EDIT_CMPT_H_
