#ifndef _ESHADER_TOOLBAR_PANEL_H_
#define _ESHADER_TOOLBAR_PANEL_H_

#include <drag2d.h>

#include "SliderItem.h"

namespace eshader
{

class Uniform;
class SliderCtrl;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage);
	virtual ~ToolbarPanel();

	void AddUniform(const std::string& title, const std::string& name, 
		Uniform* uniform, const std::vector<SliderItemFloat>& items);

	const std::vector<SliderCtrl*>& GetSliders() const { return m_sliders; }

	void Clear();

protected:
	virtual wxSizer* initLayout();

private:
	std::vector<SliderCtrl*> m_sliders;

}; // ToolbarPanel

}

#endif // _ESHADER_TOOLBAR_PANEL_H_