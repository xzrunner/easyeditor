#ifndef _LIBSHAPE_NODE_CAPTURE_CMPT_H_
#define _LIBSHAPE_NODE_CAPTURE_CMPT_H_

#include "INodeCapture.h"

#include <drag2d.h>

namespace libshape
{

class NodeCaptureCMPT : public d2d::AbstractEditCMPT, public INodeCapture
{
public:
	NodeCaptureCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel);

	//
	// interface INodeCapture
	//
	virtual float GetScope() const;

protected:
	virtual wxSizer* initLayout();

private:
	wxSlider* m_slider;

}; // NodeCaptureCMPT

}

#endif // _LIBSHAPE_NODE_CAPTURE_CMPT_H_