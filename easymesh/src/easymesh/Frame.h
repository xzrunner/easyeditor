#ifndef _EASYMESH_FRAME_H_
#define _EASYMESH_FRAME_H_

#include <ee/Frame.h>

namespace emesh
{

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title, const std::string& filetag);

protected:
	virtual void OnSaveAs(wxCommandEvent& event);

private:
	void OnSetBackground(wxCommandEvent& event);

private:
	enum
	{
		ID_SET_BG = 1100,
	};

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYMESH_FRAME_H_