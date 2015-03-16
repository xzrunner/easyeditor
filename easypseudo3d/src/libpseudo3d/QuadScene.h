#ifndef _EASYPSEUDO3D_QUAD_SCENE_H_
#define _EASYPSEUDO3D_QUAD_SCENE_H_

#include "IScene.h"

#include <drag2d.h>

namespace epseudo3d
{

class QuadScene : public IScene
{
public:
	QuadScene();
	virtual ~QuadScene();

	virtual void Load();
	virtual void Draw() const;

private:
	d2d::Image* m_img;

}; // QuadScene

}

#endif // _EASYPSEUDO3D_QUAD_SCENE_H_