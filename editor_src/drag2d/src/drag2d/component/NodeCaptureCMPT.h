#pragma once

#include "AbstractEditCMPT.h"
 
namespace d2d
{
	class MultiShapesImpl;
	class PropertySettingPanel;

	template<class T>
	class NodeCaptureCMPT : public AbstractEditCMPT
	{
	public:
		NodeCaptureCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel);

		int getNodeCaptureDistance() const;

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSlider* m_tolSlider;

	}; // NodeCaptureCMPT

	template<class T>
	inline NodeCaptureCMPT<T>::NodeCaptureCMPT(wxWindow* parent, const wxString& name, 
		EditPanel* editPanel, MultiShapesImpl* shapesImpl, PropertySettingPanel* propertyPanel)
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

