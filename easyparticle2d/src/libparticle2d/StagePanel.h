#ifndef _EASYPARTICLE2D_STAGEPANEL_H_
#define _EASYPARTICLE2D_STAGEPANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/Matrix.h>

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

	virtual bool Update(int version);

	void SetPSMat(const ee::Vector& pos);
	const ee::Matrix& GetPSMat() const { return m_ps_mat; }

public:
	ParticleSystem* m_ps;

private:
	ee::Matrix m_ps_mat;

}; // StagePanel

}

#endif // _EASYPARTICLE2D_STAGEPANEL_H_
