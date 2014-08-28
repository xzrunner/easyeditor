#ifndef D2D_SHADER_NEW_H
#define D2D_SHADER_NEW_H

#include "common/Color.h"
#include "common/tools.h"

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
	static const int MAX_COMMBINE = 1024;
	static const int SPRITE_FLOAT_NUM = 24;	// 

private:
	static ShaderNew* m_instance;

private:
	int m_prog_curr;
	int m_prog_sprite;
	int m_prog_shape;
	int m_prog_font;

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

}; // ShaderNew

}

#endif // D2D_SHADER_NEW_H