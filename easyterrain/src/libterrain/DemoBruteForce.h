#ifndef _EASYTERRAIN_DEMO_BRUTE_FORCE_H_
#define _EASYTERRAIN_DEMO_BRUTE_FORCE_H_

#include "IDemo.h"

#include <easy3d.h>

#include "BruteForce.h"

namespace libterrain
{

class DemoBruteForce : public IDemo
{
public:
	DemoBruteForce(e3d::Camera& cam);

	virtual void Load();
	virtual void Draw() const;

private:
	e3d::Camera& m_cam;

	BruteForce m_bf;

}; // BruteForce

}

#endif // _EASYTERRAIN_DEMO_BRUTE_FORCE_H_