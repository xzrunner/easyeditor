#ifndef _EASYPSEUDO3D_MAPPING_3D_SCENE_H_
#define _EASYPSEUDO3D_MAPPING_3D_SCENE_H_

#include "IScene.h"

namespace ee { class Image; }

namespace epseudo3d
{

class StageCanvas3D;

class Mapping3DScene : public IScene
{
public:
	Mapping3DScene(StageCanvas3D* canvas);
	virtual ~Mapping3DScene();

	virtual void Store(const char* filename) const override;
	virtual void Load(const char* filename) override;

	virtual void Draw() const override;
	virtual void DebugDraw() const override;

private:
	StageCanvas3D* m_canvas;

	ee::Image* m_img;

	std::vector<sm::vec3> m_vertices;
	std::vector<sm::vec2> m_texcoords;

}; // Mapping3DScene

}

#endif // _EASYPSEUDO3D_MAPPING_3D_SCENE_H_