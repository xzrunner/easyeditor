#ifndef _EASYSHADER_TOOLBAR_PANEL_H_
#define _EASYSHADER_TOOLBAR_PANEL_H_

#include "SliderItem.h"

#include <ee/ToolbarPanel.h>

namespace ee { class EditPanel; }

namespace eshader
{

class Shader;
class Uniform;
class SliderCtrl;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::EditPanel* stage);
	virtual ~ToolbarPanel();

	void AddUniform(const std::string& title, const std::string& name, 
		Shader* shader, Uniform* uniform, const std::vector<SliderItemInt>& items);
	void AddUniform(const std::string& title, const std::string& name, 
		Shader* shader, Uniform* uniform, const std::vector<SliderItemFloat>& items);

	const std::vector<SliderCtrl*>& GetSliders() const { return m_sliders; }

	void Clear();

protected:
	virtual wxSizer* InitLayout();

private:
	void AddSlider(SliderCtrl* slider);

private:
	std::vector<SliderCtrl*> m_sliders;

}; // ToolbarPanel

}

#endif // _EASYSHADER_TOOLBAR_PANEL_H_