#ifndef _EASYTERRAIN_FRAME_H_
#define _EASYTERRAIN_FRAME_H_

#include <drag2d.h>

namespace eterrain
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title);
	virtual ~Frame();

protected:
	virtual void onNew(wxCommandEvent& event);
	virtual void onOpen(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onSaveAs(wxCommandEvent& event);

#ifdef _DEBUG
private:
	wxLogChain* m_log_chain;
#endif

}; // Frame

}

#endif // _EASYTERRAIN_FRAME_H_
