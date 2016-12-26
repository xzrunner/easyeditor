#include "FilterShader.h"
#include "Utility.h"
#include "SubjectMVP2.h"
#include "EdgeDetectProg.h"
#include "ReliefProg.h"
#include "OutlineProg.h"
#include "GrayProg.h"
#include "BlurProg.h"
#include "GaussianBlurHoriProg.h"
#include "GaussianBlurVertProg.h"
#include "HeatHazeProg.h"
#include "ShockWaveProg.h"
#include "SwirlProg.h"
#include "BurningMapProg.h"
#include "ColGradingProg.h"
#include "ShaderType.h"
#include "ShaderMgr.h"
#include "RenderBuffer.h"
#include "RenderShader.h"
#include "ColorAddMul.h"
#include "StackAllocator.h"

#include <unirender/RenderContext.h>

#ifdef EASY_EDITOR
#define HAS_TEXTURE_SIZE
#endif // EASY_EDITOR

namespace sl
{

static const int MAX_COMMBINE = 1024;

FilterShader::FilterShader(ur::RenderContext* rc)
	: Shader(rc)
	, m_time(0)
	, m_curr_mode(FM_NULL)
	, m_texid(0)
	, m_quad_sz(0)
	, m_index_buf(NULL)
	, m_prog_type(0)
{
	m_vertex_buf = new Vertex[MAX_COMMBINE * 4];

	m_rc->SetClearFlag(ur::MASKC);

	m_color = 0xffffffff;
	m_additive = 0x00000000;

	InitVAList();
	InitProgs();
}

FilterShader::~FilterShader()
{
	if (m_index_buf) {
		m_index_buf->RemoveReference();
	}

	for (int i = 0; i < PROG_COUNT; ++i) {
		FilterProgram* prog = m_programs[i];
		if (prog) {
			delete prog;
		}
	}
}

void FilterShader::Bind() const
{
	if (m_curr_mode != FM_NULL) {
		int idx = m_mode2index[m_curr_mode];
		ShaderMgr::Instance()->BindRenderShader(m_programs[idx]->GetShader(), FILTER);

		m_programs[idx]->Bind();
	}
}

void FilterShader::UnBind() const
{
}

void FilterShader::Commit() const
{
	if (m_quad_sz == 0 || m_curr_mode == FM_NULL) {
		return;
	}

	int idx = m_mode2index[m_curr_mode];
	FilterProgram* prog = NULL;
	switch (m_prog_type)
	{
	case PT_NULL:
		prog = m_programs[idx];
		break;
	case PT_MULTI_ADD_COLOR:
		prog = m_programs_with_color[idx];
		if (!prog) {
			prog = InitProgWithColor(idx);
		}
		break;
	}
	if (!prog) {
        m_quad_sz = 0;
        m_prog_type = 0;
		return;
	}

	m_rc->BindTexture(m_texid, 0);
	RenderShader* shader = prog->GetShader();
	ShaderMgr::Instance()->BindRenderShader(shader, FILTER);

	int vertex_sz = prog->GetVertexSize();
	int vb_count = m_quad_sz * 4;
	int buf_sz = vertex_sz * vb_count;
	StackAllocator* alloc = StackAllocator::Instance();
	alloc->Reserve(buf_sz);
	void* buf = alloc->Alloc(buf_sz);
	uint8_t* ptr = (uint8_t*)buf;
	for (int i = 0; i < vb_count; ++i) {
		memcpy(ptr, &m_vertex_buf[i].vx, vertex_sz);
		ptr += vertex_sz;
	}

	shader->Draw(buf, vb_count, NULL, m_quad_sz * 6);
	alloc->Free(buf);

	m_quad_sz = 0;

	m_prog_type = 0;

	shader->Commit();
}

void FilterShader::SetColor(uint32_t color, uint32_t additive)
{
	m_color = color;
	m_additive = additive;
}

void FilterShader::UpdateTime(float dt)
{
	m_time += dt;
	UpdateTime();
}

void FilterShader::ClearTime()
{
	m_time = 0;
	UpdateTime();
}

void FilterShader::SetMode(FILTER_MODE mode)
{
	if (mode != m_curr_mode) {
		Commit();
		m_curr_mode = mode;
		int idx = m_mode2index[m_curr_mode];
		ShaderMgr::Instance()->BindRenderShader(m_programs[idx]->GetShader(), FILTER);
	}
}

FilterProgram* FilterShader::GetProgram(FILTER_MODE mode)
{
	int idx = m_mode2index[mode];
	if (idx >= 0 && idx < PROG_COUNT) {
		return m_programs[idx];
	} else {
		return NULL;
	}
}

void FilterShader::Draw(const float* positions, const float* texcoords, int texid) const
{
	if (m_quad_sz >= MAX_COMMBINE || (m_texid != texid && m_texid != 0)) {
		Commit();
	}
	m_texid = texid;

	bool has_multi_add = (m_color != 0xffffffff) || ((m_additive & 0xffffff) != 0);
	if (has_multi_add) {
		m_prog_type |= PT_MULTI_ADD_COLOR;
	}

	for (int i = 0; i < 4; ++i) 
	{
		Vertex* v = &m_vertex_buf[m_quad_sz * 4 + i];
		v->vx = positions[i * 2];
		v->vy = positions[i * 2 + 1];
		v->tx = texcoords[i * 2];
		v->ty = texcoords[i * 2 + 1];
		v->color = m_color;
		v->additive = m_additive;
	}
	++m_quad_sz;
}

void FilterShader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 2, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
	m_va_list[COLOR].Assign("color", 4, sizeof(uint8_t));
	m_va_list[ADDITIVE].Assign("additive", 4, sizeof(uint8_t));
}

void FilterShader::InitProgs()
{
	memset(m_programs, 0, sizeof(m_programs));
	memset(m_programs_with_color, 0, sizeof(m_programs_with_color));

	std::vector<ur::VertexAttrib> va_list;
	va_list.push_back(m_va_list[POSITION]);
	va_list.push_back(m_va_list[TEXCOORD]);

	int max_vertex = MAX_COMMBINE * 4;
	m_index_buf = Utility::CreateQuadIndexBuffer(m_rc, MAX_COMMBINE);

#ifdef HAS_TEXTURE_SIZE
	// edge detect
	EdgeDetectProg* edge_detect			= new EdgeDetectProg(m_rc, max_vertex, va_list, m_index_buf);
	edge_detect->SetBlend(0.5f);
	m_programs[PI_EDGE_DETECTION]		= edge_detect;
	// relief
	m_programs[PI_RELIEF]				= new ReliefProg(m_rc, max_vertex, va_list, m_index_buf);
	// outline
	m_programs[PI_OUTLINE]				= new OutlineProg(m_rc, max_vertex, va_list, m_index_buf);
#endif // HAS_TEXTURE_SIZE

	// gray
	m_programs[PI_GRAY]					= new GrayProg(m_rc, max_vertex, va_list, m_index_buf);
	// blur
	BlurProg* blur						= new BlurProg(m_rc, max_vertex, va_list, m_index_buf);
	blur->SetRadius(1);
	m_programs[PI_BLUR]					= blur;
	// gaussian blur
	GaussianBlurHoriProg* gbh			= new GaussianBlurHoriProg(m_rc, max_vertex, va_list, m_index_buf);
	gbh->SetTexWidth(1024);
	m_programs[PI_GAUSSIAN_BLUR_HORI]	= gbh;
	GaussianBlurVertProg* gbv			= new GaussianBlurVertProg(m_rc, max_vertex, va_list, m_index_buf);
	gbv->SetTexHeight(1024);
	m_programs[PI_GAUSSIAN_BLUR_VERT]	= gbv;

	// heat haze
	HeatHazeProg* heat_haze				= new HeatHazeProg(m_rc, max_vertex, va_list, m_index_buf);
	heat_haze->SetFactor(0.02f, 0.2f);
	m_programs[PI_HEAT_HAZE]			= heat_haze;
	// shock wave
	ShockWaveProg* shock_wave			= new ShockWaveProg(m_rc, max_vertex, va_list, m_index_buf);
	{
		float center[2] = { 0.5f, 0.5f };
		shock_wave->SetCenter(center);
		float params[3] = { 10, 0.8f, 0.1f };
		shock_wave->SetFactor(params);
	}
	m_programs[PI_SHOCK_WAVE]			= shock_wave;
#ifdef HAS_TEXTURE_SIZE
	// swirl
	SwirlProg* swirl					= new SwirlProg(m_rc, max_vertex, va_list, m_index_buf);
	{
		float center[2] = { 400, 300 };
		swirl->SetCenter(center);
		swirl->SetAngle(0.8f);
		swirl->SetRadius(200);
	}	
	m_programs[PI_SWIRL]				= swirl;
#endif // HAS_TEXTURE_SIZE
	// burning map
	BurningMapProg* burn_map			= new BurningMapProg(m_rc, max_vertex, va_list, m_index_buf);
	m_programs[PI_BURNING_MAP]			= burn_map;
	burn_map->SetLifeTime(2);

	// color grading
	ColGradingProg* col_grading			= new ColGradingProg(m_rc, max_vertex, va_list, m_index_buf);
	m_programs[PI_COL_GRADING]			= col_grading;

	memset(m_mode2index, 0xff, sizeof(m_mode2index));
	m_mode2index[FM_EDGE_DETECTION]		= PI_EDGE_DETECTION;
	m_mode2index[FM_RELIEF]				= PI_RELIEF;
	m_mode2index[FM_OUTLINE]			= PI_OUTLINE;
	m_mode2index[FM_GRAY]				= PI_GRAY;
	m_mode2index[FM_BLUR]				= PI_BLUR;
	m_mode2index[FM_GAUSSIAN_BLUR_HORI]	= PI_GAUSSIAN_BLUR_HORI;
	m_mode2index[FM_GAUSSIAN_BLUR_VERT]	= PI_GAUSSIAN_BLUR_VERT;
	m_mode2index[FM_HEAT_HAZE]			= PI_HEAT_HAZE;
	m_mode2index[FM_SHOCK_WAVE]			= PI_SHOCK_WAVE;
	m_mode2index[FM_SWIRL]				= PI_SWIRL;
	m_mode2index[FM_BURNING_MAP]		= PI_BURNING_MAP;
	m_mode2index[FM_COL_GRADING]		= PI_COL_GRADING;

	for (int i = 0; i < PROG_COUNT; ++i) {
		ShaderProgram* prog = m_programs[i];
		if (prog) {
			SubjectMVP2::Instance()->Register(prog->GetMVP());
			prog->GetShader()->SetDrawMode(ur::DRAW_TRIANGLES);
		}
	}
}

FilterProgram* FilterShader::InitProgWithColor(int idx) const
{
	std::vector<ur::VertexAttrib> va_list;
	va_list.push_back(m_va_list[POSITION]);
	va_list.push_back(m_va_list[TEXCOORD]);
	va_list.push_back(m_va_list[COLOR]);
	va_list.push_back(m_va_list[ADDITIVE]);

	FilterProgram* prog = NULL;

	int max_vertex = MAX_COMMBINE * 4;
	switch (idx)
	{
	case PI_GRAY:
		prog = new GrayProg(m_rc, max_vertex, va_list, m_index_buf, new parser::ColorAddMul());
		m_programs_with_color[PI_GRAY] = prog;
		break;
	}

	if (prog) {
		SubjectMVP2::Instance()->Register(prog->GetMVP());
		prog->GetShader()->SetDrawMode(ur::DRAW_TRIANGLES);
	}

	return prog;
}

void FilterShader::UpdateTime()
{
	for (int i = 0; i < PROG_COUNT; ++i) 
	{
		FilterProgram* prog = m_programs[i];
		if (prog) {
			prog->UpdateTime(m_time);
		}

		FilterProgram* prog_col = m_programs_with_color[i];
		if (prog_col) {
			prog_col->UpdateTime(m_time);
		}
	}
}

}