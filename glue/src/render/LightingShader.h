#ifndef _GLUE_LIGHTING_SHADER_H_
#define _GLUE_LIGHTING_SHADER_H_

#include "IShader.h"

#include "math/Vector.h"

#include <sm.h>

#include <vector>

#include <stdint.h>

struct m3_model;

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
	void SetModelView(const sm_mat4& mat);

	void SetNormalMatrix(const sm_mat3& mat);
	void SetLightPosition(const vec3& pos);

	void Draw(m3_model* model);

private:
	sm_mat4 m_mat;

}; // LightingShader

}

#endif // _GLUE_LIGHTING_SHADER_H_