#pragma once

#include <wx/wx.h>
#include <vector>

namespace d2d
{
	class EditPanel;
	class AbstractEditOP;

	class AbstractEditCMPT : public wxScrolledWindow
	{
	public:
		AbstractEditCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel,
			const wxString& childrenName = wxEmptyString, bool vertical = true);
		virtual ~AbstractEditCMPT();

		virtual void updateControlValue() {}

	protected:
		virtual wxSizer* initLayout() = 0;

		virtual void show(bool show) {}

		void addChild(AbstractEditCMPT* child) { m_children.push_back(child); }

		wxSizer* initChildrenLayout();

		void setChoice(size_t index);

	private:
		void loadEditOP();
		void loadEditOP(AbstractEditCMPT* cmpt);

		void onChangeEditType(wxCommandEvent& event);

	private:
		// wxRadioBox + Spacer
		static const int CMPT_SIZER_INDEX_OFFSET = 2;

	protected:
		EditPanel* m_editPanel;

		AbstractEditOP* m_editOP;

	private:
		wxString m_name;

		bool m_vertical;

		wxString m_childrenName;
		std::vector<AbstractEditCMPT*> m_children;
		wxSizer* m_childrenSizer;

		wxRadioBox* m_editChoice;

	}; // AbstractEditCMPT
}

