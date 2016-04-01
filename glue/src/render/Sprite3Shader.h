#ifndef _GLUE_SPRITE3_SHADER_H_
#define _GLUE_SPRITE3_SHADER_H_

#include "IShader.h"
#include "BlendMode.h"

#include "math/Vector.h"

#include <vector>

#include <stdint.h>

struct c25_camera;

namespace glue
{

class Sprite3Shader : public IShader
{
public:
	Sprite3Shader();
	virtual ~Sprite3Shader();

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

private:
	c25_camera* m_cam;

}; // Sprite3Shader

}

#endif // _GLUE_SPRITE3_SHADER_H_