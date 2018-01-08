#include "DemoBruteForce.h"

#include <SM_Vector.h>
#include <node3/Camera.h>

namespace eterrain3d
{

DemoBruteForce::DemoBruteForce(n3::Camera& cam)
	: m_cam(cam)
{
}

void DemoBruteForce::Load()
{
	m_cam.Pitch(-60);
	m_cam.MoveToward(8);

	//load the height map in
//	// 1. load from image
//	m_bf.LoadHeightMap("demo02/height128.raw", 128);
	// 2. gen 1
	m_bf.MakeTerrainPlasma(128, 1.0f);
//	// 3. gen 2
//	m_bf.MakeTerrainFault(128, 8, 0, 1, 0.4);

	m_bf.SetHeightScale(0.025f);
}

void DemoBruteForce::Draw() const
{
	m_bf.Draw();
}

}