#include "DemoBruteForce.h"

namespace libterrain
{

DemoBruteForce::DemoBruteForce(e3d::Camera& cam)
	: m_cam(cam)
{
}

void DemoBruteForce::Load()
{
	m_cam.SetPosition(vec3(6, -12, 12));
	m_cam.Rotate(0, 60);

//	m_bf.LoadHeightMap("demo02/height128.raw", 128);
	m_bf.MakeTerrainPlasma(128, 1.0f);
//	m_bf.MakeTerrainFault(128, 8, 0, 1, 0.4);

	m_bf.SetHeightScale(0.025f);
}

void DemoBruteForce::Draw() const
{
	m_bf.Draw();
}

}