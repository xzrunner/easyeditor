#ifndef _E3D_SHAPE_SHADER_H_
#define _E3D_SHAPE_SHADER_H_

#include <drag2d.h>

#include "Quaternion.h"

namespace e3d
{

class ShapeShader : public d2d::IShader
{
public:
	ShapeShader();

	virtual void Load();
	virtual void Unload();

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	void SetModelView(const mat4& mat);

	void SetColor(const d2d::Colorf& col);

protected:
	virtual void BindAttrib(GLuint prog) {}

private:
	GLuint m_model_view, m_projection;

	GLuint m_color;

	mat4 m_translation;
	mat4 m_mat_modelview, m_mat_projection;

}; // ShapeShader

}

#endif // _E3D_SHAPE_SHADER_H_