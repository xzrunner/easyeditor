#include "ShapeShader.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include "ShaderMgr.h"
#include "RenderShader.h"
#include "PositionTrans.h"
#include "VaryingNode.h"
#include "FragColor.h"
#include "AttributeNode.h"

#include <unirender/RenderContext.h>

namespace sl
{

ShapeShader::ShapeShader(ur::RenderContext* rc)
	: Shader(rc)
	, m_prog(NULL)
	, m_color(0xffffffff)
{
	m_rc->SetClearFlag(ur::MASKC);	
}

ShapeShader::~ShapeShader()
{
	delete m_prog;
}

void ShapeShader::Bind() const
{
	ShaderMgr::Instance()->BindRenderShader(m_prog->GetShader(), SHAPE2);
}

void ShapeShader::UnBind() const
{
}

void ShapeShader::Commit() const
{
	m_prog->GetShader()->Commit();
}

void ShapeShader::SetColor(uint32_t color)
{
	m_color = color;
}

void ShapeShader::SetType(int type)
{
	m_prog->GetShader()->SetDrawMode((ur::DRAW_MODE)type);
}

void ShapeShader::InitProg(int position_sz, int max_vertex)
{
	m_prog = new ShaderProgram(m_rc, max_vertex);

	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT4, "color")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color")));

	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, "color"));
	frag->Connect(new parser::FragColor());

	std::vector<ur::VertexAttrib> va_list;
	va_list.push_back(ur::VertexAttrib("position", position_sz, sizeof(float)));
	va_list.push_back(ur::VertexAttrib("color", 4, sizeof(uint8_t)));

	m_prog->Load(vert, frag, va_list, NULL, true);

	InitMVP(m_prog->GetMVP());
}

} 