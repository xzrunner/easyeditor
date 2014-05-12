#ifndef _LIBSHAPE_NODE_CAPTURE_CMPT_H_
#define _LIBSHAPE_NODE_CAPTURE_CMPT_H_

#include <drag2d.h>

namespace libshape
{

template<class T>
class NodeCaptureCMPT : public d2d::AbstractEditCMPT
{
public:
	NodeCaptureCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel);

	int getNodeCaptureDistance() const;

protected:
	virtual wxSizer* initLayout();

private:
	wxSlider* m_tolSlider;

}; // NodeCaptureCMPT

template<class T>
inline NodeCaptureCMPT<T>::NodeCaptureCMPT(wxWindow* parent, const wxString& name, 
	d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl, d2d::PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new T(editPanel, shapesImpl, propertyPanel, this);
}

template<class T>
int NodeCaptureCMPT<T>::getNodeCaptureDistance() const
{
	return m_tolSlider->GetValue();
}

template<class T>
wxSizer* NodeCaptureCMPT<T>::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Node Capture"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	m_tolSlider = new wxSlider(this, wxID_ANY, 5, 0, 15, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
	sizer->Add(m_tolSlider);
	return sizer;
}

}

#endif // _LIBSHAPE_NODE_CAPTURE_CMPT_H_