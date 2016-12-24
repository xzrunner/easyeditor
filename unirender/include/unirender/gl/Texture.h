#ifndef _UNIRENDER_GL_TEXTURE_H_
#define _UNIRENDER_GL_TEXTURE_H_

#include <CU_Uncopyable.h>

struct render;

namespace ur
{
namespace gl
{

class Texture : private cu::Uncopyable
{
public:
	Texture(render* render, void (*state_change_cb)());

	int  Create(const void* pixels, int width, int height, int format);
	void Release(int id);

	void Update(const void* pixels, int w, int h, unsigned int id);
	void UpdateSub(const void* pixels, int x, int y, int w, int h, unsigned int id);

	void Bind(int id, int channel);

	void ClearCache();

	// todo
	int  GetCurr() const { return m_textures[0]; }

private:
	static const int MAX_TEXTURE_CHANNEL = 8;

private:
	render* m_render;
	void (*m_state_change_cb)();

	int m_textures[MAX_TEXTURE_CHANNEL];

}; // Texture

}
}

#endif // _UNIRENDER_GL_TEXTURE_H_