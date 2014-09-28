#ifndef _DRAG2D_SHADER_IMPL_H_
#define _DRAG2D_SHADER_IMPL_H_

typedef unsigned int GLuint;

namespace d2d
{

class IShader;

class ShaderImpl
{
public:
	ShaderImpl();

private:
	IShader* m_curr_shader;

	IShader* m_shape_shader;
	IShader* m_sprite_shader;
	IShader* m_font_shader;

	int m_version;

	friend class ShaderNew;

}; // ShaderImpl

}

#endif // _DRAG2D_SHADER_IMPL_H_