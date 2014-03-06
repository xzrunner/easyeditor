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
	virtual void onSettings(wxCommandEvent& event);

private:
	void onSetBackground(wxCommandEvent& event);

private:
	enum
	{
		ID_SET_BG = 1100,
	};

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYCOMPLEX_FRAME_H_
