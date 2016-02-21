#ifndef _EASYSHADER_STAGE_PANEL3D_H_
#define _EASYSHADER_STAGE_PANEL3D_H_

#include <easysketch.h>

namespace eshader
{

class Shader;

class StagePanel3D : public esketch::StagePanel
{
public:
	StagePanel3D(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	void SetShader(Shader* shader) {
		m_shader = shader;
	}
	Shader* GetShader() { return m_shader; }

private:
	Shader* m_shader;

}; // StagePanel3D

}

#endif // _EASYSHADER_STAGE_PANEL3D_H_