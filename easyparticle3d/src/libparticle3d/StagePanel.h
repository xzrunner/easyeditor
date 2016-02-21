#ifndef _EASYPARTICLE3D_STAGEPANEL_H_
#define _EASYPARTICLE3D_STAGEPANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>
#include <ee/Matrix.h>

namespace eparticle3d
{

class LibraryPanel;
class ParticleSystem;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);
	virtual ~StagePanel();

	virtual bool Update(int version);

	void SetPSMat(const ee::Vector& pos);
	const ee::Matrix& GetPSMat() const { return m_ps_mat; }

private:
	void UpdateControl();

public:
	ParticleSystem* m_ps;

	float xRot, yRot;

private:
	ee::Matrix m_ps_mat;

}; // StagePanel

}

#endif // _EASYPARTICLE3D_STAGEPANEL_H_
