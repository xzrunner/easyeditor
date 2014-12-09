#ifndef _RT_APP_H_
#define _RT_APP_H_

#include <wx/wx.h>

namespace rt
{

class Frame;

class App : public wxApp
{
public:

	virtual bool OnInit();
	virtual int OnExit();
	virtual void SetStatusText(const wxString& text, int number = 0);

private:
	Frame* m_frame;

	DECLARE_EVENT_TABLE()

}; // App

}

#endif // _RT_APP_H_