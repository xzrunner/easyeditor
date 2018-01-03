#ifndef _EASYPSEUDO3D_PROJECTION_3D_SCENE_H_
#define _EASYPSEUDO3D_PROJECTION_3D_SCENE_H_

#include "IScene.h"

#include <ee/Image.h>

namespace epseudo3d
{

class StageCanvas3D;

class Projection3DScene : public IScene
{
public:
	Projection3DScene(StageCanvas3D* canvas);
	virtual ~Projection3DScene();

	virtual void Store(const char* filename) const override;
	virtual void Load(const char* filename) override;

	virtual void Draw() const override;
	virtual void DebugDraw() const override;

private:
	void DrawSprite(ee::Image* img, int x, int y) const;

private:
	StageCanvas3D* m_canvas;

	std::vector<ee::ImagePtr> m_images;
	std::vector<sm::vec2> m_positions;

}; // Projection3DScene

}

#endif // _EASYPSEUDO3D_PROJECTION_3D_SCENE_H_