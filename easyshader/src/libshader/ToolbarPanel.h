#ifndef _ESHADER_TOOLBAR_PANEL_H_
#define _ESHADER_TOOLBAR_PANEL_H_

#include <drag2d.h>

#include "SliderItem.h"

namespace eshader
{

class Uniform;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage);

	void AddUniform(const std::string& title, const std::string& name, 
		Uniform* uniform, const std::vector<SliderItemFloat>& items);

protected:
	virtual wxSizer* initLayout();

}; // ToolbarPanel

}

#endif // _ESHADER_TOOLBAR_PANEL_H_