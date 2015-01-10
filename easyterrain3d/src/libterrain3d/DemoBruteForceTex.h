#ifndef _EASYTERRAIN3D_DEMO_BRUTE_FORCE_TEX_H_
#define _EASYTERRAIN3D_DEMO_BRUTE_FORCE_TEX_H_

#include "IDemo.h"

#include <easy3d.h>

#include "BruteForceTex.h"

namespace libterrain3d
{

class DemoBruteForceTex : public IDemo
{
public:
	DemoBruteForceTex(e3d::Camera& cam);

	virtual void Load();
	virtual void Draw() const;

private:
	e3d::Camera& m_cam;

	BruteForceTex m_bf;

}; // BruteForce

}

#endif // _EASYTERRAIN3D_DEMO_BRUTE_FORCE_TEX_H_