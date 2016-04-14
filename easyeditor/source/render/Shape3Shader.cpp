#include "Shape3Shader.h"
#include "trans_color.h"
#include "Pseudo3DCamera.h"
#include "CameraMgr.h"

#include <shaderlab.h>

#include <gl/glew.h>

#include <string>

namespace ee
{

Shape3Shader::Shape3Shader()
{
}

void Shape3Shader::Load()
{
	sl_shape3_load();
}

void Shape3Shader::Unload()
{
	sl_shape3_unload();
}

void Shape3Shader::Bind()
{
	sl_shape3_bind();
}

void Shape3Shader::Unbind()
{
	sl_shape3_unbind();
}

void Shape3Shader::SetModelView()
{
	if (CameraMgr::Instance()->IsType(CameraMgr::PSEUDO3D)) {
		Pseudo3DCamera* cam = static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera());
		sl_shape3_modelview(cam->GetModelViewMat());
	}
}

void Shape3Shader::SetProjection(int width, int height)
{
	if (CameraMgr::Instance()->IsType(CameraMgr::PSEUDO3D)) {
		Pseudo3DCamera* cam = static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera());
		sl_shape3_projection(cam->GetProjectMat());
	}
}

void Shape3Shader::Commit()
{
	sl_shape3_commit();
}

void Shape3Shader::SetColor(const Colorf& col)
{
	sl_shape3_color(color2int(col, PT_ABGR));
}

}