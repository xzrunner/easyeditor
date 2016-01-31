#ifndef _EASYCOMPLEX_FRAME_H_
#define _EASYCOMPLEX_FRAME_H_



namespace ecomplex
{

class Frame : public ee::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

protected:
	virtual void OnSaveAs(wxCommandEvent& event);

	virtual void OnSettings(wxCommandEvent& event);

private:
	void onPreview(wxCommandEvent& event);
	void OnEJPreview(wxCommandEvent& event);
	void onSetBackground(wxCommandEvent& event);
	void onCode(wxCommandEvent& event);

	void SaveAsPNG(const std::string& filepath) const;
	void SaveAsJson(const std::string& filepath) const;

private:
	enum
	{
		ID_SET_BG = 1100,
		ID_PREVIEW,
		ID_EJ_PREVIEW,
	};

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYCOMPLEX_FRAME_H_
