#ifndef _ESHADER_STAGE_PANEL_H_
#define _ESHADER_STAGE_PANEL_H_

#include <drag2d.h>

namespace eshader
{

class Shader;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);

	void SetShader(Shader* shader) {
		m_shader = shader;
	}
	Shader* GetShader() { return m_shader; }

private:
	Shader* m_shader;

}; // StagePanel

}

#endif // _ESHADER_STAGE_PANEL_H_