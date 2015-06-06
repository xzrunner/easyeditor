#ifndef _DRAG2D_SHAPE_SHADER_H_
#define _DRAG2D_SHAPE_SHADER_H_

#include "IShader.h"

#include "common/Matrix.h"
#include "common/Color.h"

namespace d2d
{

struct Colorf;

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

	void SetModelView(const Vector& offset, float scale);

	void SetColor(const Colorf& col);
	const Colorf& GetColor() const { return m_color; }

private:
	GLuint m_model_view, m_projection;

	GLuint m_color_loc;

	Matrix m_mat_modelview, m_mat_projection;
	bool m_is_mat_dirty;

	Colorf m_color;

}; // ShapeShader

}

#endif // _DRAG2D_SHAPE_SHADER_H_