#ifndef _EASYEDITOR_SHADER_H_
#define _EASYEDITOR_SHADER_H_

#include "Color.h"

namespace ee
{

class Shader
{
public:
	static Shader* Instance();

	void color(const Colorf& multi, const Colorf& add = Colorf(0, 0, 0, 0));
	void color(float r, float g, float b, float a);

	void sprite();
	void shape();
	void null();

	void Release();

	void reload();

private:
	Shader();

	void load();
	void unload();

private:
	int init(const char *FS, const char *VS);
	int compile(const char * source, int type);

private:
	static Shader* m_instance;

private:
	int m_prog_curr;
	int m_prog_sprite;
	int m_prog_shape;
	int m_prog_font;

	// for sprite
	int m_multi_loc;
	int m_add_loc;
	// for shape
	int m_col_loc;

}; // Shader

}

#endif // _EASYEDITOR_SHADER_H_