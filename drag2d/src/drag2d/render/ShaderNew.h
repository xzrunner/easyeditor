#ifndef D2D_SHADER_NEW_H
#define D2D_SHADER_NEW_H

#include "common/Color.h"
#include "common/tools.h"
#include "common/Matrix.h"

typedef unsigned int GLuint;

namespace d2d
{

class Vector;

class ShaderImpl;

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

	void UpdateShapeMatrix();

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
	ShaderImpl* m_impl;

}; // ShaderNew

}

#endif // D2D_SHADER_NEW_H