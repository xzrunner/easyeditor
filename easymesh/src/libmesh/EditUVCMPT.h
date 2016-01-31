#ifndef _EASYMESH_EDIT_UV_CMPT_H_
#define _EASYMESH_EDIT_UV_CMPT_H_



namespace emesh
{

class StagePanel;

class EditUVCMPT : public ee::EditCMPT
{
public:
	EditUVCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	void onMove(wxCommandEvent& event);
	void onReset(wxCommandEvent& event);

private:
	StagePanel* m_stage;

}; // EditUVCMPT

}

#endif // _EASYMESH_EDIT_UV_CMPT_H_