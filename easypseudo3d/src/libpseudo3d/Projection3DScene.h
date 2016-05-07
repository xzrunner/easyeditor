#ifndef _EASYPSEUDO3D_PROJECTION_3D_SCENE_H_
#define _EASYPSEUDO3D_PROJECTION_3D_SCENE_H_

#include "IScene.h"

#include <ee/Image.h>

#include <easy3d.h>

namespace epseudo3d
{

class StageCanvas3D;

class Projection3DScene : public IScene
{
public:
	Projection3DScene(StageCanvas3D* canvas);
	virtual ~Projection3DScene();

	virtual void Store(const char* filename) const;
	virtual void Load(const char* filename);

	virtual void Draw() const;
	virtual void DebugDraw() const;

private:
	void DrawSprite(ee::Image* img, int x, int y) const;

private:
	StageCanvas3D* m_canvas;

	std::vector<ee::Image*> m_images;
	std::vector<sm::vec2> m_positions;

}; // Projection3DScene

}

#endif // _EASYPSEUDO3D_PROJECTION_3D_SCENE_H_