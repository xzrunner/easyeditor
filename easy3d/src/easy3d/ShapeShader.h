#ifndef _EASY3D_SHAPE_SHADER_H_
#define _EASY3D_SHAPE_SHADER_H_

namespace e3d
{

class ShapeShader
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

	void SetModelView(const sm::mat4& mat);

	void Draw(int type, const float* vertices, int count, 
		const s2::Color& col, bool force = true);
	void Draw(int type, const float* vertices, int count, 
		s2::Color* cols, bool force = true);
	void Draw(int type, const float* vertices, int vcount, 
		const s2::Color& col, unsigned short* indices, int icount);

protected:
	virtual void BindAttrib(GLuint prog);

private:
	void CopyVertex(const float* vertices, int count, int color);
	void CopyVertex(const float* vertices, int count, s2::Color* cols);

	void Commit(int type, unsigned short* indices, int count);

	static int PackColor(const s2::Color& col);

private:
	GLuint m_model_view, m_projection;

	GLuint m_vertex_buffer;

	sm::mat4 m_mat_modelview, m_mat_projection;

	int m_type;
	int m_count;
	float* m_vb;

}; // ShapeShader

}

#endif // _EASY3D_SHAPE_SHADER_H_