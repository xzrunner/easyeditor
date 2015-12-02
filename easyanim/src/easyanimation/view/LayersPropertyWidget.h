#ifndef _EASYANIM_LAYERS_PROPERTY_WIDGET_H_
#define _EASYANIM_LAYERS_PROPERTY_WIDGET_H_

#include <drag2d.h>

namespace eanim
{

class LayersPropertyWidget : public wxPanel, public d2d::Observer
{
public:
	LayersPropertyWidget(wxWindow* parent);

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

	void OnAddLayer(wxCommandEvent& event);
	void OnDelLayer(wxCommandEvent& event);
//	void onPlay(wxCommandEvent& event);
	void OnChangeFps(wxSpinEvent& event);

private:
	wxSpinCtrl* m_fps;

}; // LayersPropertyWidget

}

#endif // _EASYANIM_LAYERS_PROPERTY_WIDGET_H_