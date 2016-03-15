#include "ModelShader.h"

#include <sl_model.h>

#include <assert.h>

namespace glue
{

ModelShader::ModelShader()
{
}

ModelShader::~ModelShader()
{
}

void ModelShader::Load()
{
	sl_model_load();
	sl_model_modelview(0, 0, 1, 1);
}

void ModelShader::Unload()
{
	sl_model_unload();
}

void ModelShader::Bind()
{
	sl_model_bind();
}

void ModelShader::Unbind()
{
	sl_model_unbind();
}

void ModelShader::OnSize(int width, int height)
{
	sl_model_projection(width, height);
}

void ModelShader::Commit()
{
	sl_model_commit();
}

void ModelShader::Reset()
{
}

void ModelShader::SetProjection(int width, int height)
{
	sl_model_projection(width, height);
}

void ModelShader::SetModelView(const vec2& offset, float scale)
{
	sl_model_modelview(offset.x, offset.y, scale, scale);
}

void ModelShader::Draw(const std::vector<vec3>& vertices, const std::vector<vec2>& texcoords, int texid)
{
	assert(vertices.size() == texcoords.size());
	sl_model_draw(&vertices[0].x, &texcoords[0].x, texid, vertices.size());
}

}