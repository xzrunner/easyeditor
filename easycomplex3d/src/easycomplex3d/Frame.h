#ifndef _EASYCOMPLEX3D_FRAME_H_
#define _EASYCOMPLEX3D_FRAME_H_

#include <ee/Frame.h>

namespace ecomplex3d
{

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title);
	virtual ~Frame();

protected:
	virtual void OnNew(wxCommandEvent& event) override;
	virtual void OnOpen(wxCommandEvent& event) override;
	virtual void OnSave(wxCommandEvent& event) override;
	virtual void OnSaveAs(wxCommandEvent& event) override;

#ifdef _DEBUG
private:
	wxLogChain* m_log_chain;
#endif

}; // Frame

}

#endif // _EASYCOMPLEX3D_FRAME_H_
