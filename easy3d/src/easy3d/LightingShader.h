#ifndef _EASY3D_LIGHTING_SHADER_H_
#define _EASY3D_LIGHTING_SHADER_H_

#include "ModelShader.h"

namespace e3d
{
class IModel;

class LightingShader : public ModelShader
{
public:
	LightingShader();

	virtual void Bind() override;

protected:
	virtual void LoadShader() override;

	virtual void SetNormalMatrix(const sm::mat3& noraml_mat) override;
	virtual void SetMaterial(const Material& material) override;

private:
	GLuint m_diffuse_material;
	GLuint m_normal_matrix;
	GLuint m_light_position;
	GLuint m_ambient_material;
	GLuint m_specular_material;
	GLuint m_shininess;

}; // LightingShader

}

#endif // _EASY3D_LIGHTING_SHADER_H_