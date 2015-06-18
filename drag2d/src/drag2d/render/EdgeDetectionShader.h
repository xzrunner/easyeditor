#ifndef _DRAG2D_EDGE_DETECTION_SHADER_H_
#define _DRAG2D_EDGE_DETECTION_SHADER_H_

#include "SpriteShader.h"

namespace d2d
{

class EdgeDetectionShader : public SpriteShader
{
public:
	EdgeDetectionShader();

	virtual void Bind();

	void SetBlend(float blend);

protected:
	virtual void LoadShader();

private:
	GLuint m_blend;

}; // EdgeDetectionShader

}

#endif // _DRAG2D_EDGE_DETECTION_SHADER_H_