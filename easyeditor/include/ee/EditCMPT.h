#ifndef _EASYEDITOR_EDIT_CMPT_H_
#define _EASYEDITOR_EDIT_CMPT_H_

#include <wx/wx.h>

#include <vector>

namespace ee
{

class EditPanelImpl;
class EditOP;

class EditCMPT : public wxScrolledWindow
{
public:
	EditCMPT(wxWindow* parent, const std::string& name, EditPanelImpl* stage, 
		const std::string& child_name = "", bool vertical = true);
	virtual ~EditCMPT();

	virtual void UpdateControlValue() {}

	void SetEditOP(EditOP* op);
	EditOP* GetChildEditOP();		// todo

protected:
	virtual wxSizer* InitLayout() = 0;

	void AddChild(EditCMPT* child) { m_children.push_back(child); }

	wxSizer* InitChildrenLayout();

	void SetChoice(size_t index);

	void LoadEditOP(EditOP* op);

private:
	void LoadEditOP();
	void LoadEditOP(EditCMPT* cmpt);

	void OnChangeEditType(wxCommandEvent& event);

private:
	// wxRadioBox + Spacer
	static const int CMPT_SIZER_INDEX_OFFSET = 2;

protected:
	EditPanelImpl* m_stage;

	EditOP* m_editop;

private:
	std::string m_name;

	bool m_vertical;

	std::string m_child_name;
	std::vector<EditCMPT*> m_children;
	wxSizer* m_child_sz;

	wxRadioBox* m_edit_choice;

}; // EditCMPT

}

#endif // _EASYEDITOR_EDIT_CMPT_H_