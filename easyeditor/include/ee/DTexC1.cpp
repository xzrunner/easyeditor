#include "DTexC1.h"

#include <dtex_c1.h>

namespace ee
{

DTexC1::DTexC1(int tex_size)
{
	m_c1 = dtex_c1_create(tex_size);
}

DTexC1::~DTexC1()
{
	dtex_c1_release(m_c1);
}

void DTexC1::Bind()
{
	dtex_c1_bind(m_c1);
}

void DTexC1::Unbind()
{
	dtex_c1_unbind(m_c1);
}

int DTexC1::GetTextureId()
{
	return dtex_c1_get_texture_id(m_c1);
}

int DTexC1::GetTextureSize()
{
	return dtex_c1_get_texture_size(m_c1);
}

void DTexC1::Clear(float xmin, float ymin, float xmax, float ymax)
{
	dtex_c1_clear(m_c1, xmin, ymin, xmax, ymax);
}

void DTexC1::DebugDraw() const
{
	dtex_c1_debug_draw(m_c1);
}

}