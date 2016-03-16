#include "ModelShader.h"
#include "ColorTrans.h"
#include "trans_color.h"

#include <sl_model.h>

namespace ee
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
// 	sl_model_modelview(0, 0, 1, 1);
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

void ModelShader::SetModelView(const Vector& offset, float scale)
{
	sl_model_modelview(offset.x, offset.y, scale, scale);
}

void ModelShader::SetProjection(int width, int height)
{
	sl_model_projection(width, height, NEAR, FAR);
}

void ModelShader::Commit()
{
	sl_model_commit();
}

void ModelShader::Reset()
{
}

void ModelShader::SetColor(const ColorTrans& color)
{
// 	sl_model_set_color(color2int(color.multi, PT_ABGR), 
// 		color2int(color.add, PT_ABGR));
// 	sl_model_set_map_color(color2int(color.r, PT_ABGR), 
// 		color2int(color.g, PT_ABGR), 
// 		color2int(color.b, PT_ABGR));
}

void ModelShader::Draw(const std::vector<vec3>& positions, const std::vector<Vector>& texcoords, int texid)
{
	sl_model_draw(&positions[0].x, &texcoords[0].x, texid, positions.size());
}

}