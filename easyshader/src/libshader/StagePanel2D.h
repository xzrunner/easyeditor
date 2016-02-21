#ifndef _EASYSHADER_STAGE_PANEL2D_H_
#define _EASYSHADER_STAGE_PANEL2D_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

namespace eshader
{

class Shader;

class StagePanel2D : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel2D(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library);

	void SetShader(Shader* shader) {
		m_shader = shader;
	}
	Shader* GetShader() { return m_shader; }

private:
	Shader* m_shader;

}; // StagePanel2D

}

#endif // _EASYSHADER_STAGE_PANEL2D_H_