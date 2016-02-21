#ifndef _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_
#define _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_

#include "IScene.h"

#include <ee/Vector.h>

#include <vector>

namespace ee { class EditPanelImpl; class Symbol; }

namespace epseudo3d
{

class Projection2DScene : public IScene
{
public:
	Projection2DScene(ee::EditPanelImpl* stage);
	virtual ~Projection2DScene();

	virtual void Store(const char* filename) const {}
	virtual void Load(const char* filename);

	virtual void Draw() const;
	virtual void DebugDraw() const {}

private:
	void InitCamera();
	void InitEditOP();

	void CalProjInfo(const ee::Vector& src_pos, ee::Vector* dst_pos, float* dst_scale) const;

private:
	ee::EditPanelImpl* m_stage;

	std::vector<ee::Symbol*> m_buildings;
	std::vector<ee::Vector> m_positions;

}; // Projection2DScene

}

#endif // _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_