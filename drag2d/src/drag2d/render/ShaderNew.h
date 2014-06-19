#ifndef D2D_SHADER_NEW_H
#define D2D_SHADER_NEW_H

#include "common/Color.h"
#include "common/tools.h"

typedef unsigned int GLuint;

namespace d2d
{

class ShaderNew
{
public:
	static ShaderNew* Instance();

// 		void color(const Colorf& multi, const Colorf& add = Colorf(0, 0, 0, 0));
// 		void color(float r, float g, float b, float a);

	void sprite();
	void shape();
	void null();

	void release();

	void reload();

	void SetTexture(int tex);

	void Draw(const float vb[16], int texid);

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

	uint32_t m_color, m_additive;

}; // ShaderNew

}

#endif // D2D_SHADER_NEW_H