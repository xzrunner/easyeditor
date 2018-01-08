#ifndef _EASYTERRAIN3D_DEMO_BRUTE_FORCE_TEX_H_
#define _EASYTERRAIN3D_DEMO_BRUTE_FORCE_TEX_H_

#include "IDemo.h"
#include "BruteForceTex.h"

namespace n3 { class Camera; }

namespace eterrain3d
{

class DemoBruteForceTex : public IDemo
{
public:
	DemoBruteForceTex(n3::Camera& cam);

	virtual void Load() override;
	virtual void Draw() const override;

private:
	n3::Camera& m_cam;

	BruteForceTex m_bf;

}; // BruteForce

}

#endif // _EASYTERRAIN3D_DEMO_BRUTE_FORCE_TEX_H_