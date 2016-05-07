#include "ShaderMgr.h"
#include "LightingShader.h"
#include "ShapeShader.h"
#include "SpriteShader.h"

#include <ee/ShaderContext.h>

namespace e3d
{

ShaderMgr* ShaderMgr::m_instance = NULL;

ShaderMgr* ShaderMgr::Instance()
{
	if (!m_instance)
	{
		m_instance = new ShaderMgr();
		ee::ShaderContext::SetShader3DMgr(m_instance);
	}
	return m_instance;
}

ShaderMgr::ShaderMgr()
{
	m_model_shader = new LightingShader;
	m_shaders.push_back(m_model_shader);

	m_shape_shader = new ShapeShader;
	m_shaders.push_back(m_shape_shader);

	m_sprite_shader = new SpriteShader;
	m_shaders.push_back(m_sprite_shader);
}

ShaderMgr::~ShaderMgr()
{
	delete m_model_shader;
	delete m_shape_shader;
	delete m_sprite_shader;
}

void ShaderMgr::NullProg()
{
	Null();
}

void ShaderMgr::Null()
{
	ee::ShaderContext::Bind3d();
	Switch(NULL);
}

void ShaderMgr::Model()
{
	ee::ShaderContext::Bind3d();
	Switch(m_model_shader);
}

void ShaderMgr::Shape()
{
	ee::ShaderContext::Bind3d();
	Switch(m_shape_shader);
}

void ShaderMgr::Sprite()
{
	ee::ShaderContext::Bind3d();
	Switch(m_sprite_shader);	
}

void ShaderMgr::DrawModel(const IModel* model, const sm::mat4& m)
{
	m_model_shader->Draw(model, m);
}

void ShaderMgr::DrawShape(int type, const float* vertices, int count, const ee::Colorf& col, bool force)
{
	m_shape_shader->Draw(type, vertices, count, col, force);
}

void ShaderMgr::DrawShape(int type, const float* vertices, int count, ee::Colorf* cols, bool force)
{
	m_shape_shader->Draw(type, vertices, count, cols, force);
}

void ShaderMgr::DrawShape(int type, const float* vertices, int vcount, const ee::Colorf& col, 
						  unsigned short* indices, int icount)
{
	m_shape_shader->Draw(type, vertices, vcount, col, indices, icount);
}

void ShaderMgr::DrawTri(const sm::vec3 vertices[3], const sm::vec2 texcoords[3], int texid)
{
	m_sprite_shader->DrawTri(vertices, texcoords, texid);
}

void ShaderMgr::DrawTri(const float* vertices, const float* texcoords, int count, int texid)
{
	m_sprite_shader->DrawTri(vertices, texcoords, count, texid);
}

void ShaderMgr::DrawTriStrip(float* vertices, float* texcoords, int count, int texid)
{
	m_sprite_shader->DrawTriStrip(vertices, texcoords, count, texid);
}

void ShaderMgr::SetModelView(const sm::mat4& mat)
{
	m_model_shader->SetModelView(mat);
	m_shape_shader->SetModelView(mat);
	m_sprite_shader->SetModelView(mat);
}

void ShaderMgr::SetModelShader(ModelShader* shader, bool delete_old)
{
	if (m_model_shader == shader) {
		return;
	}

	std::vector<ee::IShader*>::iterator itr;
	for (itr = m_shaders.begin(); itr != m_shaders.end(); ++itr) {
		if (*itr == m_model_shader) {
			m_shaders.erase(itr);
			break;
		}
	}

	if (delete_old) {
		m_model_shader->Unload();
		delete m_model_shader;
	}
	m_model_shader = shader;

	m_shaders.push_back(m_model_shader);
}

}