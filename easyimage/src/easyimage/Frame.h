#ifndef _EASYIMAGE_FRAME_H_
#define _EASYIMAGE_FRAME_H_



namespace eimage
{

class Frame : public ee::Frame
{
public:
	Frame(const wxString& title);

protected:
	virtual void OnNew(wxCommandEvent& event);
	virtual void OnOpen(wxCommandEvent& event);
	virtual void OnSave(wxCommandEvent& event);
	virtual void OnSaveAs(wxCommandEvent& event);

}; // Frame

}

#endif // _EASYIMAGE_FRAME_H_
