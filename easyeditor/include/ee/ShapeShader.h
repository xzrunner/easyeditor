#ifndef _EASYEDITOR_SHAPE_SHADER_H_
#define _EASYEDITOR_SHAPE_SHADER_H_

#include "IShader.h"
#include "Matrix.h"
#include "Color.h"

namespace ee
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

	virtual void Reset() {}

	void SetModelView(const Vector& offset, float scale);

	void SetColor(const Colorf& col);
	const Colorf& GetColor() const { return m_color; }

protected:
	virtual void LoadShader();

private:
	GLuint m_model_view, m_projection;

	GLuint m_color_loc;

	Matrix m_mat_modelview, m_mat_projection;
	bool m_is_mat_dirty;

	Colorf m_color;

}; // ShapeShader

}

#endif // _EASYEDITOR_SHAPE_SHADER_H_