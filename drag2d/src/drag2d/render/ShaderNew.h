#ifndef D2D_SHADER_NEW_H
#define D2D_SHADER_NEW_H

#include "common/Color.h"
#include "common/tools.h"
#include "common/Matrix.h"

typedef unsigned int GLuint;

namespace d2d
{

class Vector;

class ShaderNew
{
public:
	static ShaderNew* Instance();

	void SetSpriteColor(const Colorf& multi, const Colorf& add);
	void SetShapeColor(const Colorf& col);

	void sprite();
	void shape();
	void null();

	void release();

	void reload();

	int GetTexID() const { return m_tex; }
	int GetFboID() const { return m_fbo; }

	void SetTexture(int tex);
	void SetFBO(int fbo);

	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);

	int GetVersion() const { return m_version; }

	void SetBufferData(bool open) {
		m_open_buffer_data = open;
	}
	bool IsOpenBufferData() const { return m_open_buffer_data; }

	void SetModelView(const Vector& offset, float scale);
	void SetProjection(int width, int height);

	void Flush();

private:
	ShaderNew();

	void load();
	void unload();

	int InitShader(const char *FS, const char *VS);
	int CompileShader(const char * source, int type);

	void InitBuffers();

	void Commit();

	void CopyVertex(const float vb[16]);

private:
	static int MAX_COMMBINE;
	static const int SPRITE_FLOAT_NUM = 24;	// 

private:
	static ShaderNew* m_instance;

private:
	struct RenderState
	{
		GLuint prog;
		GLuint model_view, projection;

		RenderState() {
			prog = 0;
			model_view = projection = 0;
		}
	};

private:
	GLuint m_prog_curr;
	GLuint m_prog_font;

	RenderState m_rs_sprite;
	RenderState m_rs_shape;

// 		// for sprite
// 		int m_multi_loc;
// 		int m_add_loc;
// 		// for shape
// 		int m_col_loc;

	//////////////////////////////////////////////////////////////////////////
	GLuint VertexBuffer;
	GLuint IndexBuffer;

	int m_sprite_count;

	float* m_vb;

	GLuint m_tex;
	GLuint m_fbo;

	uint32_t m_color, m_additive;

	// for shape
	int m_col_loc;

	int m_version;

	bool m_open_buffer_data;

// 	GLuint m_model_view;
// 	GLuint m_projection;

	Matrix m_mat_modelview, m_mat_projection;

}; // ShaderNew

}

#endif // D2D_SHADER_NEW_H