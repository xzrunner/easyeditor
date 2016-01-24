#ifndef _GLUE_SHADER_MGR_H_
#define _GLUE_SHADER_MGR_H_

#include "utility/macro.h"
#include "math/Vector.h"

namespace glue
{

class IShader;
class SpriteShader;
class ShapeShader;

class ShaderMgr
{
public:
	void Init();

	void OnSize(int width, int height);

	void SpriteDraw(const vec2 vertices[4], const vec2 texcoords[4], int texid);
	void ShapeDraw();

	void Flush();

private:
	void ChangeShader(IShader* shader);

private:
	IShader* m_curr_shader;

	SpriteShader* m_sprite;
	ShapeShader* m_shape;

	SINGLETON_DECLARATION(ShaderMgr)

}; // ShaderMgr

}

#endif // _GLUE_SHADER_MGR_H_