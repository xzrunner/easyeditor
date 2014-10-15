#ifndef _E3D_SHADER_MGR_H_
#define _E3D_SHADER_MGR_H_

#include <drag2d.h>

#include "sketch/Quaternion.h"

namespace z3d { class IModel; }

namespace e3d
{

class ShaderMgr : public d2d::ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	void Lighting();

	void Draw(const z3d::IModel* model, const vec3& pos);

	void SetModelView(const Quaternion& ori);

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	d2d::LightingShader* m_light_shader;

}; // ShaderMgr

}

#endif // _E3D_SHADER_MGR_H_