#ifndef _EASYANIM_LAYERS_PANEL_H_
#define _EASYANIM_LAYERS_PANEL_H_

#include <wx/wx.h>

namespace eanim
{

class LayersPropertyWidget;

class LayersPanel : public wxScrolledWindow
{
public:
	LayersPanel(wxWindow* parent);

//		void onPlay();

private:
	void InitLayout();

private:
	LayersPropertyWidget* m_layersProperty;

	wxWindow* m_contentPanel;

}; // LayersPanel

}

#endif // _EASYANIM_LAYERS_PANEL_H_