#ifndef _EASYPSEUDO3D_MAPPING_3D_SCENE_H_
#define _EASYPSEUDO3D_MAPPING_3D_SCENE_H_

#include "IScene.h"

#include <drag2d.h>
#include <easy3d.h>

namespace epseudo3d
{

class StageCanvas;

class Mapping3DScene : public IScene
{
public:
	Mapping3DScene(StageCanvas* canvas);
	virtual ~Mapping3DScene();

	virtual void Store(const char* filename) const;
	virtual void Load(const char* filename);

	virtual void Draw() const;
	virtual void DebugDraw() const;

private:
	StageCanvas* m_canvas;

	d2d::Image* m_img;

	std::vector<vec3> m_vertices;
	std::vector<vec2> m_texcoords;

}; // Mapping3DScene

}

#endif // _EASYPSEUDO3D_MAPPING_3D_SCENE_H_