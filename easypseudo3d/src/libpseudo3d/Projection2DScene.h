#ifndef _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_
#define _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_

#include "IScene.h"

#include <drag2d.h>

namespace epseudo3d
{

class Projection2DScene : public IScene
{
public:
	Projection2DScene(d2d::EditPanel* stage);
	virtual ~Projection2DScene();

	virtual void Store(const char* filename) const {}
	virtual void Load(const char* filename);

	virtual void Draw() const;
	virtual void DebugDraw() const {}

private:
	void InitCamera();
	void InitEditOP();

	void CalProjInfo(const d2d::Vector& src_pos, d2d::Vector* dst_pos, float* dst_scale) const;

private:
	d2d::EditPanel* m_stage;

	std::vector<d2d::ISymbol*> m_buildings;
	std::vector<d2d::Vector> m_positions;

}; // Projection2DScene

}

#endif // _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_