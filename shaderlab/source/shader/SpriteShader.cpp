#include "SpriteShader.h"
#include "ObserverMVP.h"
#include "Utility.h"
#include "ShaderProgram.h"
#include "ShaderMgr.h"
#include "RenderBuffer.h"
#include "RenderShader.h"
#include "RenderLayout.h"
#include "Shader.h"
#include "PositionTrans.h"
#include "AttributeNode.h"
#include "VaryingNode.h"
#include "TextureMap.h"
#include "FragColor.h"
#include "ColorAddMul.h"
#include "ColorMap.h"
#include "Buffer.h"

#include <unirender/RenderContext.h>

namespace sl
{

SpriteShader::SpriteShader(ur::RenderContext* rc, int position_sz, int max_vertex,
						   bool vertex_index)
	: Shader(rc)
	, m_max_vertex(max_vertex)
	, m_vertex_index(vertex_index)
{
	m_rc->SetClearFlag(ur::MASKC);

	m_color = 0xffffffff;
	m_additive = 0x00000000;
	m_rmap = 0x000000ff;
	m_gmap = 0x0000ff00;
	m_bmap = 0x00ff0000;

	m_texid = 0;

	m_quad_sz = 0;

	m_prog_type = 0;

	InitVAList(position_sz);
}

SpriteShader::~SpriteShader()
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		delete m_programs[i];
	}
}

void SpriteShader::Bind() const
{
	// todo bind PT_NO_COLOR ? 
}

void SpriteShader::UnBind() const
{
//	Commit();

// 	ur::RenderContext* ctx = ShaderMgr::Instance()->GetContext();
// 	ctx->SetDefaultBlend();
}

void SpriteShader::SetColor(uint32_t color, uint32_t additive)
{
	m_color = color;
	m_additive = additive;
}

void SpriteShader::SetColorMap(uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	m_rmap = rmap;
	m_gmap = gmap;
	m_bmap = bmap;
}

void SpriteShader::InitProgs()
{
	RenderBuffer* idx_buf = NULL;
	if (m_vertex_index) {
		idx_buf = Utility::CreateQuadIndexBuffer(m_rc, m_max_vertex / 4);
	}
	InitNoColorProg(idx_buf);
	InitMultiAddColorProg(idx_buf);
	InitMapColorProg(idx_buf);
	InitFullColorProg(idx_buf);
	if (m_vertex_index) {
		idx_buf->RemoveReference();
	}
}

void SpriteShader::InitVAList(int position_sz)
{
	m_va_list[POSITION].Assign("position", position_sz, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
	m_va_list[COLOR].Assign("color", 4, sizeof(uint8_t));
	m_va_list[ADDITIVE].Assign("additive", 4, sizeof(uint8_t));
	m_va_list[RMAP].Assign("rmap", 4, sizeof(uint8_t));
	m_va_list[GMAP].Assign("gmap", 4, sizeof(uint8_t));
	m_va_list[BMAP].Assign("bmap", 4, sizeof(uint8_t));
}

ShaderProgram* SpriteShader::CreateProg(parser::Node* vert, parser::Node* frag, 
										const std::vector<VA_TYPE>& va_types, RenderBuffer* ib) const
{
	ShaderProgram* prog = new ShaderProgram(m_rc, m_max_vertex);

	std::vector<ur::VertexAttrib> va_list;
	for (int i = 0, n = va_types.size(); i < n; ++i) {
		va_list.push_back(m_va_list[va_types[i]]);
	}

	prog->Load(vert, frag, va_list, ib, true);

	InitMVP(prog->GetMVP());

	prog->GetShader()->SetDrawMode(ur::DRAW_TRIANGLES);

	return prog;
}

void SpriteShader::InitNoColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	m_programs[PI_NO_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitMultiAddColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "additive")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorAddMul())->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(COLOR);
	va_types.push_back(ADDITIVE);
	m_programs[PI_MULTI_ADD_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitMapColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "bmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "bmap")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorMap())->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(RMAP);
	va_types.push_back(GMAP);
	va_types.push_back(BMAP);
	m_programs[PI_MAP_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void SpriteShader::InitFullColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "additive")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "rmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "gmap")))->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "bmap")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "bmap")));

	parser::Node* frag = new parser::TextureMap();
	frag->Connect(
		new parser::ColorMap())->Connect(
		new parser::ColorAddMul())->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(COLOR);
	va_types.push_back(ADDITIVE);
	va_types.push_back(RMAP);
	va_types.push_back(GMAP);
	va_types.push_back(BMAP);
	m_programs[PI_FULL_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

}