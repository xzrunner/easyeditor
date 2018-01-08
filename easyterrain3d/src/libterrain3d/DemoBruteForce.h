#ifndef _EASYTERRAIN3D_DEMO_BRUTE_FORCE_H_
#define _EASYTERRAIN3D_DEMO_BRUTE_FORCE_H_

#include "IDemo.h"

#include "BruteForce.h"

namespace n3 { class Camera; }

namespace eterrain3d
{

class DemoBruteForce : public IDemo
{
public:
	DemoBruteForce(n3::Camera& cam);

	virtual void Load() override;
	virtual void Draw() const override;

private:
	n3::Camera& m_cam;

	BruteForce m_bf;

}; // BruteForce

}

#endif // _EASYTERRAIN3D_DEMO_BRUTE_FORCE_H_