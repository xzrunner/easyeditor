#ifndef _GLUE_MODEL_SHADER_H_
#define _GLUE_MODEL_SHADER_H_

#include "IShader.h"
#include "BlendMode.h"

#include "math/Vector.h"

#include <vector>

#include <stdint.h>

namespace glue
{

class ModelShader : public IShader
{
public:
	ModelShader();
	virtual ~ModelShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void OnSize(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetProjection(int width, int height);
	void SetModelView(const vec2& offset, float scale);

	void Draw(const std::vector<vec3>& vertices, const std::vector<vec2>& texcoords, int texid);

public:
	static const int FAR = 50;
	static const int NEAR = 2;

}; // ModelShader

}

#endif // _GLUE_MODEL_SHADER_H_