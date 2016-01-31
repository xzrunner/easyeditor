#ifndef _EASYEDITOR_LAYER_DIALOG_H_
#define _EASYEDITOR_LAYER_DIALOG_H_

#include <wx/wx.h>

namespace ee
{

class Layer;

class LayerDialog : public wxDialog
{
public:
	LayerDialog(wxWindow* parent, Layer* layer, const wxPoint& pos);

private:
	void InitLayout();

	void OnChangeVisible(wxCommandEvent& event);
	void OnChangeEditable(wxCommandEvent& event);

private:
	Layer* m_layer;

	wxCheckBox* m_check_view;
	wxCheckBox* m_check_edit;

}; // LayerDialog

}

#endif // _EASYEDITOR_LAYER_DIALOG_H_