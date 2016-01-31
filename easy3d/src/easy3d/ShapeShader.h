#ifndef _E3D_SHAPE_SHADER_H_
#define _E3D_SHAPE_SHADER_H_



#include "Matrix.h"

namespace e3d
{

class ShapeShader : public ee::IShader
{
public:
	ShapeShader();
	virtual ~ShapeShader();

	virtual void Load();
	virtual void Unload();

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset() {}

	void SetModelView(const mat4& mat);

	void Draw(int type, const float* vertices, int count, 
		const ee::Colorf& col, bool force = true);
	void Draw(int type, const float* vertices, int count, 
		ee::Colorf* cols, bool force = true);
	void Draw(int type, const float* vertices, int vcount, 
		const ee::Colorf& col, unsigned short* indices, int icount);

protected:
	virtual void BindAttrib(GLuint prog);

private:
	void CopyVertex(const float* vertices, int count, int color);
	void CopyVertex(const float* vertices, int count, ee::Colorf* cols);

	void Commit(int type, unsigned short* indices, int count);

	static int PackColor(const ee::Colorf& col);

private:
	GLuint m_model_view, m_projection;

	GLuint m_vertex_buffer;

	mat4 m_mat_modelview, m_mat_projection;

	int m_type;
	int m_count;
	float* m_vb;

}; // ShapeShader

}

#endif // _E3D_SHAPE_SHADER_H_