#include "RenderShader.h"
#include "RenderBuffer.h"
#include "RenderLayout.h"
#include "ShaderMgr.h"
#include "Shader.h"
#include "Statistics.h"

#include <unirender/RenderContext.h>

//#define SHADER_LOG
//#define SL_DC_STAT

#ifdef SHADER_LOG
#include <iostream>
#endif // SHADER_LOG

#ifdef SL_DC_STAT
#include <iostream>
#endif // SL_DC_STAT

namespace sl
{

RenderShader::RenderShader(ur::RenderContext* rc)
	: m_rc(rc)
{
	m_prog = 0;

	m_texture_number = 0;

	m_uniform_number = 0;
	m_uniform_changed = false;

	m_vb = m_ib = NULL;
	m_layout = NULL;

	m_draw_mode = ur::DRAW_POINTS;
}

RenderShader::~RenderShader()
{
	if (m_vb) m_vb->RemoveReference();
	if (m_ib) m_ib->RemoveReference();
	if (m_layout) m_layout->RemoveReference();
}

void RenderShader::Load(const char* vs, const char* fs)
{
#ifdef SHADER_LOG
	std::cout << "================================================== \n";
	std::cout << vs << '\n';
	std::cout << fs << '\n';
	std::cout << "================================================== \n";
#endif // SHADER_LOG

	m_prog = m_rc->CreateShader(vs, fs);
	m_rc->BindShader(m_prog);
//	render_shader_bind(m_ej_render, 0);	// ??
	//	S->curr_shader = -1;
}

void RenderShader::Unload()
{
	m_rc->ReleaseShader(m_prog);
}

void RenderShader::SetVertexBuffer(RenderBuffer* vb) 
{ 
	RefCountObjAssign(m_vb, vb);
}

void RenderShader::SetIndexBuffer(RenderBuffer* ib) 
{ 
	RefCountObjAssign(m_ib, ib);
}

void RenderShader::SetLayout(RenderLayout* lo) 
{ 
	RefCountObjAssign(m_layout, lo);
}

void RenderShader::Bind()
{
	m_rc->BindShader(m_prog);
	m_vb->Bind();
	if (m_ib) {
		m_ib->Bind();
	}
//	m_layout->Bind();
}

void RenderShader::Commit()
{
	if (!m_vb || m_vb->IsEmpty()) {
		return;
	}

	ApplyUniform();

	Statistics* stat = Statistics::Instance();

	m_vb->Update();
	if (m_ib) {
		m_ib->Update();
		m_rc->DrawElements((ur::DRAW_MODE)m_draw_mode, 0, m_ib->Size());
		stat->AddVertices(m_ib->Size());
		m_ib->Clear();
	} else {
		m_rc->DrawArrays((ur::DRAW_MODE)m_draw_mode, 0, m_vb->Size());
		stat->AddVertices(m_vb->Size());
	}
	m_vb->Clear();

	stat->AddDrawCall();
}

void RenderShader::SetDrawMode(DRAW_MODE_TYPE dm) 
{ 
	if (m_draw_mode != dm) {
#ifdef SL_DC_STAT
		std::cout << "SL DC for set draw mode\n";
#endif // SL_DC_STAT
		Commit();
		m_draw_mode = dm;
	}
}

int RenderShader::AddUniform(const char* name, UNIFORM_FORMAT_TYPE t)
{
	// todo ur::RenderContext::Bind()

	if (m_uniform_number >= MAX_UNIFORM) {
		return -1;
	}
	int loc = m_rc->GetShaderUniform(name);
	int index = m_uniform_number++;
	m_uniform[index].Assign(loc, t);
	return loc < 0 ? -1 : index;
}

void RenderShader::SetUniform(int index, UNIFORM_FORMAT_TYPE t, const float* v)
{
	// todo ur::RenderContext::Bind()

	if (index < 0 || index >= m_uniform_number) {
		return;
	}

	bool same = m_uniform[index].Same(t, v);
	if (same) {
		return;
	}

	m_uniform_changed = true;
	if (Shader* shader = ShaderMgr::Instance()->GetShader()) {
#ifdef SL_DC_STAT
		std::cout << "SL DC for set uniform\n";
#endif // SL_DC_STAT
		shader->Commit();
	}
	m_uniform[index].Assign(t, v);
}

void RenderShader::Draw(void* vb, int vb_n, void* ib, int ib_n)
{
	if (m_ib && ib_n > 0 && m_ib->Add(ib, ib_n)) {
#ifdef SL_DC_STAT
		std::cout << "ib over\n";
#endif // SL_DC_STAT
		Commit();
	}
	if (m_vb && vb_n > 0 && m_vb->Add(vb, vb_n)) {
#ifdef SL_DC_STAT
		std::cout << "vb over\n";
#endif // SL_DC_STAT
		Commit();
	}
}

void RenderShader::ApplyUniform()
{
	for (int i = 0; i < m_uniform_number; ++i) {
		bool changed = m_uniform[i].Apply(m_rc);
		if (changed) {
			m_uniform_changed = changed;
		}
	}
}

int RenderShader::GetUniformSize(UNIFORM_FORMAT_TYPE t)
{
	int n = 0;
	switch(t) {
	case ur::UNIFORM_INVALID:
		n = 0;
		break;
	case ur::UNIFORM_FLOAT1:
		n = 1;
		break;
	case ur::UNIFORM_FLOAT2:
		n = 2;
		break;
	case ur::UNIFORM_FLOAT3:
		n = 3;
		break;
	case ur::UNIFORM_FLOAT4:
		n = 4;
		break;
	case ur::UNIFORM_FLOAT33:
		n = 9;
		break;
	case ur::UNIFORM_FLOAT44:
		n = 16;
		break;
	case ur::UNIFORM_INT1:
		n = 1;
		break;
	}
	return n;
}

/************************************************************************/
/* RenderShader::Uniform                                                */
/************************************************************************/

RenderShader::Uniform::Uniform() 
	: m_loc(0), m_type(0), m_changed(false) 
{
	memset(m_value, 0, sizeof(m_value));
}

bool RenderShader::Uniform::Same(UNIFORM_FORMAT_TYPE t, const float* v)
{
	assert(t == m_type);
	int n = GetUniformSize(t);
	int change = memcmp(m_value, v, n * sizeof(float));
	if (change != 0) {
		return false;
	} else {
		return true;
	}
}

void RenderShader::Uniform::Assign(int loc, UNIFORM_FORMAT_TYPE type) 
{
	this->m_loc = loc;
	this->m_type = type;
	m_changed = false;
	memset(m_value, 0, sizeof(m_value));
}

void RenderShader::Uniform::Assign(UNIFORM_FORMAT_TYPE t, const float* v) 
{
	assert(t == m_type);
	int n = GetUniformSize(t);
	memcpy(m_value, v, n * sizeof(float));
	m_changed = true;
}

bool RenderShader::Uniform::Apply(ur::RenderContext* rc) 
{
	if (m_changed && m_loc >= 0) {
		rc->SetShaderUniform(m_loc, (ur::UNIFORM_FORMAT)m_type, m_value);
		return true;
	} else {
		return false;
	}
}

}