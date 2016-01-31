#ifndef _EASYMAP_FRAME_H_
#define _EASYMAP_FRAME_H_



namespace emap
{

class Frame : public ee::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

private:
	void OnPreview(wxCommandEvent& event);
	void OnExtendSetting(wxCommandEvent& event);

private:
	enum
	{
		ID_PREVIEW = 1100,
		ID_SETING_EXTEND,
	};

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYMAP_FRAME_H_
