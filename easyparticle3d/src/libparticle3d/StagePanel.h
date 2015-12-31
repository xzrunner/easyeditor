#ifndef _EASYPARTICLE3D_STAGEPANEL_H_
#define _EASYPARTICLE3D_STAGEPANEL_H_

#include <drag2d.h>

namespace eparticle3d
{

class LibraryPanel;
class ParticleSystem;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);
	virtual ~StagePanel();

	virtual bool Update(int version);

	void SetPSMat(const d2d::Vector& pos);
	const d2d::Matrix& GetPSMat() const { return m_ps_mat; }

private:
	void UpdateControl();

public:
	ParticleSystem* m_ps;

	float xRot, yRot;

private:
	d2d::Matrix m_ps_mat;

}; // StagePanel

}

#endif // _EASYPARTICLE3D_STAGEPANEL_H_
