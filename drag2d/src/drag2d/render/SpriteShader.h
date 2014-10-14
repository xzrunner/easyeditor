#ifndef _DRAG2D_SPRITE_SHADER_H_
#define _DRAG2D_SPRITE_SHADER_H_

#include "IShader.h"

#include "common/Matrix.h"

typedef unsigned __int32 uint32_t;

namespace d2d
{

struct Colorf;

class SpriteShader : public IShader
{
public:
	SpriteShader();
	virtual ~SpriteShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetModelView(const Vector& offset, float scale);
	virtual void SetModelView(const Quaternion& ori) {}
	virtual void SetProjection(int width, int height);

	virtual void Commit();

	int GetTexID() const;
	void SetTexID(int tex);

	int GetFboID() const;
	void SetFboID(int fbo);

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetColor(const Colorf& multi, const Colorf& add);

	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);

protected:
	virtual void BindAttrib(GLuint prog);

private:
	void InitBuffers();

	void CopyVertex(const float vb[16]);

private:
	static int MAX_COMMBINE;
	static const int SPRITE_FLOAT_NUM = 24;

private:
	GLuint m_model_view, m_projection;

	GLuint m_vertex_buffer;
	GLuint m_index_buffer;

	int m_count;
	float* m_vb;

	uint32_t m_color, m_additive;

	GLuint m_tex;
	GLuint m_fbo;

	bool m_open_buffer_data;

	Matrix m_mat_modelview, m_mat_projection;
	bool m_is_mat_dirty;

}; // SpriteShader

}

#endif // _DRAG2D_SPRITE_SHADER_H_