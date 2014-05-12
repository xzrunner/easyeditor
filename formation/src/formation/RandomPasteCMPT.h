#ifndef FORMATION_RANDOM_PASTE_CMPT_H
#define FORMATION_RANDOM_PASTE_CMPT_H

#include <drag2d.h>

namespace formation
{
	class RandomPasteCMPT : public d2d::AbstractEditCMPT
	{
	public:
		RandomPasteCMPT(wxWindow* parent, const wxString& name, 
			d2d::EditPanel* editPanel);

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