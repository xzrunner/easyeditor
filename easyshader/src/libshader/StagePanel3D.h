#ifndef _ESHADER_STAGE_PANEL3D_H_
#define _ESHADER_STAGE_PANEL3D_H_

#include <easysketch.h>

namespace eshader
{

class Shader;

class StagePanel3D : public libsketch::StagePanel
{
public:
	StagePanel3D(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::LibraryPanel* library);

	void SetShader(Shader* shader) {
		m_shader = shader;
	}
	Shader* GetShader() { return m_shader; }

private:
	Shader* m_shader;

}; // StagePanel3D

}

#endif // _ESHADER_STAGE_PANEL3D_H_