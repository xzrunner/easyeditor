#ifndef _EASYCOMPLEX_FRAME_H_
#define _EASYCOMPLEX_FRAME_H_

#include <drag2d.h>

namespace ecomplex
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

protected:
	virtual void onSaveAs(wxCommandEvent& event);

	virtual void onSettings(wxCommandEvent& event);

private:
	void onPreview(wxCommandEvent& event);
	void onEJPreview(wxCommandEvent& event);
	void onSetBackground(wxCommandEvent& event);

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
