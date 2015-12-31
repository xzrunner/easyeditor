#ifndef _EASYPARTICLE2D_STAGEPANEL_H_
#define _EASYPARTICLE2D_STAGEPANEL_H_

#include <drag2d.h>

namespace eparticle2d
{

class LibraryPanel;
class ToolbarPanel;
class Symbol;
class ParticleSystem;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);

	virtual bool Update(int version);

	void SetPSMat(const d2d::Vector& pos);
	const d2d::Matrix& GetPSMat() const { return m_ps_mat; }

public:
	ParticleSystem* m_ps;

private:
	d2d::Matrix m_ps_mat;

}; // StagePanel

}

#endif // _EASYPARTICLE2D_STAGEPANEL_H_
