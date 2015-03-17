#ifndef _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_
#define _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_

#include "IScene.h"

#include <drag2d.h>

namespace epseudo3d
{

class Projection2DScene : public IScene
{
public:
	Projection2DScene();
	virtual ~Projection2DScene();

	virtual void Store(const char* filename) const {}
	virtual void Load(const char* filename);

	virtual void Draw() const;
	virtual void DebugDraw() const {}

private:
	std::vector<d2d::ISprite*> m_sprites;

}; // Projection2DScene

}

#endif // _EASYPSEUDO3D_PROJECTION_2D_SCENE_H_