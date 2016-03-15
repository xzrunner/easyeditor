#ifndef _GLUE_SHADER_MGR_H_
#define _GLUE_SHADER_MGR_H_

#include "BlendMode.h"

#include "utility/macro.h"
#include "math/Vector.h"

#include <vector>

#include <stdint.h>

namespace glue
{

class IShader;

class ShaderMgr
{
public:
	void Init();

	void OnSize(int width, int height);

	void SpriteDraw(const vec2 vertices[4], const vec2 texcoords[4], int texid);
	void ShapeDraw();
	void BlendDraw(const vec2 vertices[4], const vec2 texcoords[4], 
		const vec2 texcoords_base[4], int tex_blend, int tex_base);
	void ModelDraw(const std::vector<vec3>& vertices, const std::vector<vec2>& texcoords, int texid);
	
	void SetSpriteColor(uint32_t color, uint32_t additive);
	void SetSpriteMapColor(uint32_t rmap, uint32_t gmap, uint32_t bmap);

	void SetBlendColor(uint32_t color, uint32_t additive);
	void SetBlendMode(BlendMode mode);

	bool IsBlendShader() const;

	void Flush();

private:
	void ChangeShader(int idx);

private:
	std::vector<IShader*> m_shaders;

	int m_curr_shader;
	
	SINGLETON_DECLARATION(ShaderMgr)

}; // ShaderMgr

}

#endif // _GLUE_SHADER_MGR_H_