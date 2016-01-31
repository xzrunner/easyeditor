#ifndef FORMATION_RANDOM_PASTE_CMPT_H
#define FORMATION_RANDOM_PASTE_CMPT_H



namespace formation
{
	class RandomPasteCMPT : public ee::AbstractEditCMPT
	{
	public:
		RandomPasteCMPT(wxWindow* parent, const wxString& name, 
			ee::EditPanel* editPanel);

		void updateByLibrary();

	protected:
		virtual wxSizer* initLayout();

	private:
		void initChoice();

		void onChoiceChanged(wxCommandEvent& event);
		void onNumberChanged(wxSpinEvent& event);

	private:
		wxChoice* m_choice;

		wxSpinCtrl* m_number;

		std::map<wxString, int> m_mapNameToNum;

		friend class RandomPasteOP;

	}; // RandomPasteCMPT
}

#endif // FORMATION_RANDOM_PASTE_CMPT_H