#include "DemoBruteForceTex.h"

namespace libterrain
{

DemoBruteForceTex::DemoBruteForceTex(e3d::Camera& cam)
	: m_cam(cam)
{
}

void DemoBruteForceTex::Load()
{
	m_cam.SetPosition(vec3(6, -12, 12));
	m_cam.Rotate(0, 60);

	//load the height map in
//	// 1. load from image
//	m_bf.LoadHeightMap("demo02/height128.raw", 128);
	// 2. gen 1
	m_bf.MakeTerrainPlasma(128, 1.0f);
//	// 3. gen 2
//	m_bf.MakeTerrainFault(128, 8, 0, 1, 0.4);

	m_bf.SetHeightScale(0.025f);

	//load the terrain texture in
	m_bf.LoadTexture("demo03/grass_1.tga");
}

void DemoBruteForceTex::Draw() const
{
	m_bf.Draw();
}

}