#ifndef _EASY2D_SHAPE_SHADER_H_
#define _EASY2D_SHAPE_SHADER_H_

#include "IShader.h"

#include "common/Matrix.h"

struct Colorf;

namespace e2d
{

class ShapeShader : public IShader
{
public:
	ShapeShader();

	virtual void Load();
	virtual void Unload();

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	void SetModelView(const vec2& offset, float scale);

	void SetColor(const Colorf& col);

protected:
	virtual void BindAttrib(GLuint prog) {}

private:
	GLuint m_model_view, m_projection;

	GLuint m_color;

	Matrix m_mat_modelview, m_mat_projection;
	bool m_is_mat_dirty;

}; // ShapeShader

}

#endif // _EASY2D_SHAPE_SHADER_H_