#ifndef _EASYANIM_FRAME_H_
#define _EASYANIM_FRAME_H_



namespace eanim
{

class Frame : public ee::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

protected:
	virtual void OnOpen(wxCommandEvent& event);
	virtual void OnSaveAs(wxCommandEvent& event);

private:
	void OnPreview(wxCommandEvent& event);
	void OnSetBackground(wxCommandEvent& event);
	void OnCodeSetting(wxCommandEvent& event);
	void OnCodeLove2d(wxCommandEvent& event);

	void SaveAsPNG(const std::string& filepath) const;
	void SaveAsSingle(const std::string& filepath) const;
	void SaveAsTemplate(const std::string& filepath) const;

private:
	enum
	{
		ID_PREVIEW = 2000,
		ID_BACKGROUND,
		ID_CODESETTING,
		ID_LOVE2D
	};

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYANIM_FRAME_H_