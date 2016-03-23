#ifndef _GLUE_LIGHTING_SHADER_H_
#define _GLUE_LIGHTING_SHADER_H_

#include "IShader.h"

#include "math/Vector.h"
#include "math/Matrix.h"

#include <vector>

#include <stdint.h>

struct m3_mesh;

namespace glue
{

class LightingShader : public IShader
{
public:
	LightingShader();
	virtual ~LightingShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void OnSize(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetProjection(int width, int height);
	void SetModelView(const mat4& mat);

	void SetMaterial(const vec3& ambient, const vec3& diffuse, 
		const vec3& specular, float shininess);
	void SetNormalMatrix(const mat3& mat);
	void SetLightPosition(const vec3& pos);

	void Draw(m3_mesh* mesh);

private:
	mat4 m_mat;

}; // LightingShader

}

#endif // _GLUE_LIGHTING_SHADER_H_