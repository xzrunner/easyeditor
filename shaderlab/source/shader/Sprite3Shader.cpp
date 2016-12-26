#include "Sprite3Shader.h"
#include "SubjectMVP3.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include "ShaderMgr.h"
#include "RenderShader.h"
#include "RenderBuffer.h"
#include "StackAllocator.h"

#include <unirender/RenderContext.h>

#include <assert.h>

namespace sl
{

static const int MAX_VERTICES = 4096;

Sprite3Shader::Sprite3Shader(ur::RenderContext* rc)
	: SpriteShader(rc, 3, MAX_VERTICES, false)
{
	InitProgs();
	m_vertex_buf = new Vertex[MAX_VERTICES];
}

void Sprite3Shader::Commit() const
{
	if (m_quad_sz == 0) {
		return;
	}

	m_rc->BindTexture(m_texid, 0);

	ShaderProgram* prog = NULL;
	switch (m_prog_type)
	{
	case PT_NULL:
		prog = m_programs[PI_NO_COLOR];
		break;
	case PT_MULTI_ADD_COLOR:
		prog = m_programs[PI_MULTI_ADD_COLOR];
		break;
	case PT_MAP_COLOR:
		prog = m_programs[PI_MAP_COLOR];
		break;
	default:
		assert((m_prog_type & PT_MULTI_ADD_COLOR) && (m_prog_type & PT_MAP_COLOR));
		prog = m_programs[PI_FULL_COLOR];
	}

	int vertex_sz = prog->GetVertexSize();
	int vb_count = m_quad_sz * 6;
	int buf_sz = vertex_sz * vb_count;
	StackAllocator* alloc = StackAllocator::Instance();
	alloc->Reserve(buf_sz);
	void* buf = alloc->Alloc(buf_sz);
	uint8_t* ptr = (uint8_t*)buf;
	if (m_prog_type == PT_MAP_COLOR) {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, sizeof(float) * 5);
			ptr += sizeof(float) * 5;
			memcpy(ptr, &m_vertex_buf[i].color, sizeof(uint32_t) * 2);
			ptr += sizeof(uint32_t) * 2;
		}
	} else {
		for (int i = 0; i < vb_count; ++i) {
			memcpy(ptr, &m_vertex_buf[i].vx, vertex_sz);
			ptr += vertex_sz;
		}
	}

	RenderShader* shader = prog->GetShader();
	ShaderMgr::Instance()->BindRenderShader(shader, SPRITE3);
	shader->Draw(buf, vb_count);
	alloc->Free(buf);

	m_quad_sz = 0;

	m_prog_type = 0;

	shader->Commit();
}

void Sprite3Shader::Draw(const float* positions, const float* texcoords, int texid) const
{
	if (m_quad_sz * 6 >= MAX_VERTICES || (m_texid != texid && m_texid != 0)) {
		Commit();
	}
	m_texid = texid;

	bool has_multi_add = (m_color != 0xffffffff) || (m_additive != 0);
	bool has_map = ((m_rmap & 0x00ffffff) != 0x000000ff) || ((m_gmap & 0x00ffffff) != 0x0000ff00) || ((m_bmap & 0x00ffffff) != 0x00ff0000);
	if (has_multi_add) {
		m_prog_type |= PT_MULTI_ADD_COLOR;
	}
	if (has_map) {
		m_prog_type |= PT_MAP_COLOR;
	}

	for (int i = 0; i < 6; ++i) 
	{
		Vertex* v = &m_vertex_buf[m_quad_sz * 6 + i];
		v->vx = positions[i * 3];
		v->vy = positions[i * 3 + 1];
		v->vz = positions[i * 3 + 2];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
		v->color	= m_color;
		v->additive = m_additive;
		v->rmap = m_rmap;
		v->gmap = m_gmap;
		v->bmap = m_bmap;
	}
	++m_quad_sz;
}

void Sprite3Shader::InitMVP(ObserverMVP* mvp) const
{
	SubjectMVP3::Instance()->Register(mvp);
}

}