#ifndef _EASYANIM_LAYERS_PANEL_H_
#define _EASYANIM_LAYERS_PANEL_H_

#include <wx/wx.h>

namespace eanim
{

class Controller;
class LayersPropertyWidget;

class LayersPanel : public wxPanel
{
public:
	LayersPanel(wxWindow* parent, Controller* ctrl);

//		void onPlay();

private:
	void InitLayout();

private:
	Controller* m_ctrl;

	LayersPropertyWidget* m_layersProperty;

	wxWindow* m_contentPanel;

}; // LayersPanel

}

#endif // _EASYANIM_LAYERS_PANEL_H_