#ifndef _SG_FRAME_H_
#define _SG_FRAME_H_



namespace sg
{

class Frame : public ee::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

private:
	void onChangePerspective(wxCommandEvent& event);

	void onSetBackground(wxCommandEvent& event);

private:
	enum
	{
		ID_CHANGE_PERSPECTIVE = 1100,
		ID_SET_BG = 1101
	};

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _SG_FRAME_H_