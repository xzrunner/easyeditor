#ifndef _EASYIMAGE_FRAME_H_
#define _EASYIMAGE_FRAME_H_

#include <ee/Frame.h>

namespace eimage
{

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title);

protected:
	virtual void OnNew(wxCommandEvent& event) override;
	virtual void OnOpen(wxCommandEvent& event) override;
	virtual void OnSave(wxCommandEvent& event) override;
	virtual void OnSaveAs(wxCommandEvent& event) override;

}; // Frame

}

#endif // _EASYIMAGE_FRAME_H_
