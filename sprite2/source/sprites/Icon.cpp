#include "Icon.h"
#include "Texture.h"
#include "RenderParams.h"

#include <shaderlab.h>

namespace s2
{

Icon::Icon()
	: m_tex(NULL)
{
}

Icon::Icon(const Icon& icon)
	: m_tex(icon.m_tex)
{
}

Icon& Icon::operator = (const Icon& icon)
{
	m_tex = icon.m_tex;
	return *this;
}

void Icon::Draw(const RenderParams& params, float process) const
{
	if (!m_tex) {
		return;
	}

	// texid
	int texid = m_tex->GetTexID();

	// texcoords
	sm::vec2 texcoords[4];
	GenTexcoords(process, texcoords);

	// vertices
	sm::vec2 vertices[4];
	GenVertices(process, texcoords, vertices);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = params.mt * vertices[i];
	}

	// draw
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
}

sm::rect Icon::GetRegion(float process) const
{
	sm::rect ret;
	if (!m_tex) {
		return ret;
	}

	sm::vec2 texcoords[4];
	GenTexcoords(process, texcoords);

	sm::vec2 vertices[4];
	GenVertices(process, texcoords, vertices);
	for (int i = 0; i < 4; ++i) {
		float x = vertices[i].x,
			  y = vertices[i].y;
		if (x < ret.xmin) ret.xmin = x;
		if (x > ret.xmax) ret.xmax = x;
		if (y < ret.ymin) ret.ymin = y;
		if (y > ret.ymax) ret.ymax = y;
	}

	return ret;
}

void Icon::GenTexcoords(float process, sm::vec2* texcoords) const
{
	sm::vec2 quad[4];
	GetQuad(process, quad);
	for (int i = 0; i < 4; ++i) {
		texcoords[i].x = quad[i].x;
		texcoords[i].y = quad[i].y;
	}
}

void Icon::GenVertices(float process, const sm::vec2* texcoords, 
					   sm::vec2* vertices) const
{
	sm::vec2 sz = m_tex->GetSize();
	for (int i = 0; i < 4; ++i) {
		vertices[i].x = (texcoords[i].x - 0.5f) * sz.x;
		vertices[i].y = (texcoords[i].y - 0.5f) * sz.y;
	}
}

}