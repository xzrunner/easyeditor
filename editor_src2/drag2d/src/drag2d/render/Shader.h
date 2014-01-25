#ifndef D2D_SHADER_H
#define D2D_SHADER_H

#include "common/Color.h"

namespace d2d
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

	void release();

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

	// for sprite
	int m_multi_loc;
	int m_add_loc;
	// for shape
	int m_col_loc;

}; // Shader
}

#endif // D2D_SHADER_H