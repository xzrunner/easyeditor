#ifndef _EASYPARTICLE2D_STAGEPANEL_H_
#define _EASYPARTICLE2D_STAGEPANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

#include <SM_Matrix.h>

namespace eparticle2d
{

class LibraryPanel;
class ToolbarPanel;
class Symbol;
class ParticleSystem;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);

	virtual bool UpdateStage();

	void SetPSMat(const sm::vec2& pos);
	const sm::mat4& GetPSMat() const { return m_ps_mat; }

public:
	ParticleSystem* m_ps;

private:
	sm::mat4 m_ps_mat;

}; // StagePanel

}

#endif // _EASYPARTICLE2D_STAGEPANEL_H_
