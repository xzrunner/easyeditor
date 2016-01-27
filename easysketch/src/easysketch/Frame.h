#ifndef _EASYSKETCH_FRAME_H_
#define _EASYSKETCH_FRAME_H_

#include <drag2d.h>

namespace esketch
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title);
	virtual ~Frame();

protected:
	virtual void OnNew(wxCommandEvent& event);
	virtual void OnOpen(wxCommandEvent& event);
	virtual void OnSave(wxCommandEvent& event);
	virtual void OnSaveAs(wxCommandEvent& event);

#ifdef _DEBUG
private:
	wxLogChain* m_log_chain;
#endif

}; // Frame

}

#endif // _EASYSKETCH_FRAME_H_
