#include "Model3Shader.h"
#include "SubjectMVP3.h"
#include "ObserverMVP.h"
#include "ShaderProgram.h"
#include "Utility.h"
#include "ShaderType.h"
#include "ShaderMgr.h"
#include "RenderShader.h"
#include "RenderBuffer.h"
#include "PositionTrans.h"
#include "AttributeNode.h"
#include "VaryingNode.h"
#include "FragColor.h"
#include "GouraudShading.h"
#include "VaryingNode.h"
#include "TextureMap.h"
#include "Assign.h"
#include "Mul2.h"
#include "StackAllocator.h"

#include <unirender/RenderContext.h>

#include <ds_array.h>

namespace sl
{

static const int MAX_VERTICES = 10000;
static const int MAX_INDICES = 20000;

Model3Shader::Model3Shader(ur::RenderContext* rc)
	: Shader(rc)
	, m_curr_shader(-1)
{
	m_rc->SetClearFlag(ur::MASKC | ur::MASKD);

	InitVAList();
	InitProgs();
}

Model3Shader::~Model3Shader()
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		delete m_programs[i];
	}
}

void Model3Shader::Bind() const
{
}

void Model3Shader::UnBind() const
{
}

void Model3Shader::Commit() const
{
	if (m_curr_shader < 0) {
		return;
	}

	RenderShader* shader = m_programs[m_curr_shader]->GetShader();
	m_rc->SetDepth(ur::DEPTH_LESS_EQUAL);
	shader->Commit();
	m_rc->SetDepth(ur::DEPTH_DISABLE);
}

void Model3Shader::SetMaterial(const sm::vec3& ambient, const sm::vec3& diffuse, 
							   const sm::vec3& specular, float shininess, int tex)
{
	m_shading_uniforms.SetMaterial(m_programs[PI_GOURAUD_SHADING]->GetShader(), ambient, diffuse, specular, shininess);
	m_shading_uniforms.SetMaterial(m_programs[PI_GOURAUD_TEXTURE]->GetShader(), ambient, diffuse, specular, shininess);
	if (tex >= 0) {
		m_rc->SetDepth(ur::DEPTH_LESS_EQUAL);
		m_rc->BindTexture(tex, 0);
	}
}

void Model3Shader::SetLightPosition(const sm::vec3& pos)
{
	m_programs[PI_GOURAUD_SHADING]->GetShader()->SetUniform(m_shading_uniforms.light_position, ur::UNIFORM_FLOAT3, &pos.x);
	m_programs[PI_GOURAUD_TEXTURE]->GetShader()->SetUniform(m_shading_uniforms.light_position, ur::UNIFORM_FLOAT3, &pos.x);
}

void Model3Shader::Draw(const ds_array* vertices, const ds_array* indices,
						bool has_normal, bool has_texcoord) const
{
	PROG_IDX idx = PI_STATIC_COLOR;
	if (!has_normal && !has_texcoord) idx = PI_STATIC_COLOR;
	if ( has_normal && !has_texcoord) idx = PI_GOURAUD_SHADING;
	if (!has_normal &&  has_texcoord) idx = PI_TEXTURE_MAP;
	if ( has_normal &&  has_texcoord) idx = PI_GOURAUD_TEXTURE;
	if (idx != m_curr_shader) {
		Commit();
		m_curr_shader = idx;
		ShaderMgr::Instance()->BindRenderShader(m_programs[idx]->GetShader(), MODEL3);
	}

	RenderShader* shader = m_programs[m_curr_shader]->GetShader();
	const RenderBuffer *vb = shader->GetVertexBuffer(),
		               *ib = shader->GetIndexBuffer();
	int vn = ds_array_size(vertices),
		in = ds_array_size(indices);
	if (vb->Size() + vn > vb->Capacity() || 
		ib->Size() + in > ib->Capacity()) {
		Commit();
	}
	if (vn > vb->Capacity() || in > ib->Capacity()) {
		return;
	}
	
	int ioffset = vb->Size();
	int isz = ds_array_size(indices);
	StackAllocator* alloc = StackAllocator::Instance();
	alloc->Reserve(sizeof(uint16_t) * isz);
	void* buf = alloc->Alloc(sizeof(uint16_t) * isz);
	uint16_t* array = (uint16_t*)buf;
	memcpy(buf, ds_array_data(indices), sizeof(uint16_t) * isz);
	for (int i = 0; i < isz; ++i) {
		array[i] += ioffset;
	}
	shader->Draw((void*)ds_array_data(vertices), vn, buf, in);
	alloc->Free(buf);
}

void Model3Shader::SetModelView(const sm::mat4& mat)
{
	for (int i = 0; i < PROG_COUNT; ++i) {
		ShaderProgram* prog = m_programs[i];
		if (prog) {
			prog->GetMVP()->SetModelview(&mat);
		}
	}

	sm::mat3 mat3(mat);
	m_programs[PI_GOURAUD_SHADING]->GetShader()->SetUniform(m_shading_uniforms.normal_matrix, ur::UNIFORM_FLOAT33, mat3.x);
	m_programs[PI_GOURAUD_TEXTURE]->GetShader()->SetUniform(m_shading_uniforms.normal_matrix, ur::UNIFORM_FLOAT33, mat3.x);
}

void Model3Shader::InitVAList()
{
	m_va_list[POSITION].Assign("position", 3, sizeof(float));
	m_va_list[TEXCOORD].Assign("texcoord", 2, sizeof(float));
	m_va_list[NORMAL].Assign("normal", 3, sizeof(float));	
}

void Model3Shader::InitProgs()
{
	RenderBuffer* idx_buf = Utility::CreateIndexBuffer(m_rc, MAX_INDICES);
	InitStaticColorProg(idx_buf);
	InitGouraudShadingProg(idx_buf);
 	InitTextureMapProg(idx_buf);
 	InitGouraudTextureProg(idx_buf);
	idx_buf->RemoveReference();
}

void Model3Shader::InitStaticColorProg(RenderBuffer* idx_buf)
{
	parser::Node* vert = new parser::PositionTrans();
	parser::Node* frag = new parser::Assign(parser::Variable(parser::VT_FLOAT4, "_col_static_"), 0.5, 0.5, 0, 1);
	frag->Connect(new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	m_programs[PI_STATIC_COLOR] = CreateProg(vert, frag, va_types, idx_buf);
}

void Model3Shader::InitGouraudShadingProg(RenderBuffer* idx_buf)
{
	std::string varying_name = "gouraud_dst";

	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::GouraudShading())->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, varying_name)));

	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, varying_name));
	frag->Connect(new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(NORMAL);
	m_programs[PI_GOURAUD_SHADING] = CreateProg(vert, frag, va_types, idx_buf);

	m_shading_uniforms.Init(m_programs[PI_GOURAUD_SHADING]->GetShader());
}

void Model3Shader::InitTextureMapProg(RenderBuffer* idx_buf)
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
	m_programs[PI_TEXTURE_MAP] = CreateProg(vert, frag, va_types, idx_buf);
}

// todo
void Model3Shader::InitGouraudTextureProg(RenderBuffer* idx_buf)
{
	const char* gouraud_dst_name = "gouraud_dst";

	parser::Node* varying = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, gouraud_dst_name));
	parser::Node* vert = new parser::PositionTrans();
	vert->Connect(
		new parser::GouraudShading())->Connect(
		varying)->Connect(
		new parser::AttributeNode(parser::Variable(parser::VT_FLOAT2, "texcoord")))->Connect(
		new parser::VaryingNode(parser::Variable(parser::VT_FLOAT2, "texcoord")));

	parser::Node* tex_map = new parser::TextureMap();
	parser::Node* frag = new parser::VaryingNode(parser::Variable(parser::VT_FLOAT4, gouraud_dst_name));
	frag->Connect(tex_map)->Connect(
		new parser::Mul2(parser::Variable(parser::VT_FLOAT4, "tmp"), varying->GetOutput(), tex_map->GetOutput()))->Connect(
		new parser::FragColor());

	std::vector<VA_TYPE> va_types;
	va_types.push_back(POSITION);
	va_types.push_back(TEXCOORD);
	va_types.push_back(NORMAL);
	m_programs[PI_GOURAUD_TEXTURE] = CreateProg(vert, frag, va_types, idx_buf);

	m_shading_uniforms.Init(m_programs[PI_GOURAUD_TEXTURE]->GetShader());
}

ShaderProgram* Model3Shader::CreateProg(parser::Node* vert, parser::Node* frag, 
										const std::vector<VA_TYPE>& va_types,
										RenderBuffer* ib) const
{
	ShaderProgram* prog = new ShaderProgram(m_rc, MAX_VERTICES);

	std::vector<ur::VertexAttrib> va_list;
	for (int i = 0, n = va_types.size(); i < n; ++i) {
		va_list.push_back(m_va_list[va_types[i]]);
	}

	prog->Load(vert, frag, va_list, ib, true);

	SubjectMVP3::Instance()->Register(prog->GetMVP());

	prog->GetShader()->SetDrawMode(ur::DRAW_TRIANGLES);

	return prog;
}

/************************************************************************/
/* class Model3Shader::GouraudUniforms                                  */
/************************************************************************/

void Model3Shader::GouraudUniforms::
Init(RenderShader* shader)
{
	diffuse = shader->AddUniform("u_diffuse_material", ur::UNIFORM_FLOAT3);
	ambient = shader->AddUniform("u_ambient_material", ur::UNIFORM_FLOAT3);
	specular = shader->AddUniform("u_specular_material", ur::UNIFORM_FLOAT3);
	shininess = shader->AddUniform("u_shininess", ur::UNIFORM_FLOAT1);

	normal_matrix = shader->AddUniform("u_normal_matrix", ur::UNIFORM_FLOAT33);
	light_position = shader->AddUniform("u_light_position", ur::UNIFORM_FLOAT3);
}

void Model3Shader::GouraudUniforms::
SetMaterial(RenderShader* shader, const sm::vec3& ambient, const sm::vec3& diffuse, 
			const sm::vec3& specular, float shininess) 
{
	shader->SetUniform(this->ambient, ur::UNIFORM_FLOAT3, &ambient.x);
	shader->SetUniform(this->diffuse, ur::UNIFORM_FLOAT3, &diffuse.x);
	shader->SetUniform(this->specular, ur::UNIFORM_FLOAT3, &specular.x);
	shader->SetUniform(this->shininess, ur::UNIFORM_FLOAT1, &shininess);
}

}