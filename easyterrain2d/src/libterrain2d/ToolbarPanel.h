#ifndef _EASYTERRAIN2D_TOOLBAR_PANEL_H_
#define _EASYTERRAIN2D_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace eterrain2d
{

class StagePanel;
class OceanMesh;
class WaveVerticesCMPT;
class MoveTexcoordsCMPT;
class BlendTextureCMPT;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

	void SetControlersValue(const OceanMesh* ocean);

protected:
	virtual wxSizer* InitLayout();

private:
	WaveVerticesCMPT* m_wave_cmpt;
	MoveTexcoordsCMPT* m_move_uv_cmpt;
	BlendTextureCMPT* m_blend_tex_cmpt;

}; // ToolbarPanel

}

#endif // _EASYTERRAIN2D_TOOLBAR_PANEL_H_