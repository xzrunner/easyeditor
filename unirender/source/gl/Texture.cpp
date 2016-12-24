#include "gl/Texture.h"

#include <ejoy2d/render.h>

#include <string>

namespace ur
{
namespace gl
{

Texture::Texture(render* render, void (*state_change_cb)())
	: m_render(render)
	, m_state_change_cb(state_change_cb)
{
	memset(m_textures, 0, sizeof(m_textures));
}

int Texture::Create(const void* pixels, int width, int height, int format)
{
	RID id = render_texture_create(m_render, width, height, static_cast<TEXTURE_FORMAT>(format), TEXTURE_2D, 0);
	render_texture_update(m_render, id, width, height, pixels, 0, 0);
	return id;
}

void Texture::Release(int id)
{
	render_release(m_render, TEXTURE, id);
}

void Texture::Update(const void* pixels, int w, int h, unsigned int id)
{
	render_texture_update(m_render, id, w, h, pixels, 0, 0);
}

void Texture::UpdateSub(const void* pixels, int x, int y, int w, int h, unsigned int id)
{
	render_texture_subupdate(m_render, id, pixels, x, y, w, h);
}

void Texture::Bind(int id, int channel)
{
	if (channel < 0 || channel >= MAX_TEXTURE_CHANNEL || m_textures[channel] == id) {
		return;
	}

	m_state_change_cb();

	m_textures[channel] = id;
	render_set(m_render, TEXTURE, id, channel);
}

void Texture::ClearCache()
{
	render_clear_texture_cache(m_render);
}

}
}