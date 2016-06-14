#ifndef _EASYCOMPLEX_FRAME_H_
#define _EASYCOMPLEX_FRAME_H_

#include <ee/Frame.h>

namespace ecomplex
{

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title, const std::string& filetag);

protected:
	virtual void OnSaveAs(wxCommandEvent& event);

	virtual void OnSettings(wxCommandEvent& event);

	virtual std::string GetFileFilter() const;

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
