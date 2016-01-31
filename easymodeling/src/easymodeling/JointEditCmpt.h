
#ifndef EMODELING_JOINT_EDIT_CMPT_H
#define EMODELING_JOINT_EDIT_CMPT_H



namespace emodeling
{
	class StagePanel;

	class JointEditCmpt : public ee::EditCMPT
	{
	public:
		JointEditCmpt(wxWindow* parent, const wxString& name,
			StagePanel* editPanel, ee::PropertySettingPanel* propertyPanel);

		virtual void updateControlValue();

	protected:
		virtual wxSizer* InitLayout();

	private:
		void onCreateJoint(wxCommandEvent& event);

		void onTypeChanged(wxCommandEvent& event);

	private:
		class SelectWheelDialog : public wxDialog
		{
		public:
			SelectWheelDialog(wxWindow* parent, const wxString& title, 
				const wxString& body0, const wxString& body1);

			int getChoice() const {
				return m_wheelChoice->GetSelection();
			}

		private:
			void InitLayout(const wxString& body0, const wxString& body1);

		private:
			wxChoice* m_wheelChoice;

		}; // SelectWheelDialog

	private:
		StagePanel* m_stage_panel;

		wxChoice* m_typeChoice;

		wxButton* m_btnOK;

	}; // JointEditCmpt
}

#endif // EMODELING_JOINT_EDIT_CMPT_H
