#ifndef _E3D_SHADER_MGR_H_
#define _E3D_SHADER_MGR_H_

#include <drag2d.h>

#include "Matrix.h"

namespace e3d
{

class IModel;
class LightingShader;
class ShapeShader;

class ShaderMgr : public d2d::ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	void Lighting();
	void Shape();

	void Draw(const IModel* model, const mat4& m);

	void SetModelView(const mat4& mat);

	void SetShapeColor(const d2d::Colorf& col);

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	LightingShader* m_light_shader;
	ShapeShader* m_shape_shader;

}; // ShaderMgr

}

#endif // _E3D_SHADER_MGR_H_