#ifndef _E3D_LIGHTING_SHADER_H_
#define _E3D_LIGHTING_SHADER_H_

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

#include <drag2d.h>

namespace e3d
{
class IModel;

class LightingShader : public d2d::IShader
{
public:
	LightingShader();
	virtual ~LightingShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void Commit();

	virtual void SetProjection(int width, int height);

	void SetModelView(const mat4& mat);

	void Draw(const IModel* model, const vec3& pos);

protected:
	virtual void BindAttrib(GLuint prog);

private:
	struct Node
	{
		const IModel* model;

		vec3 pos;
		Quaternion ori;
	};

private:
	GLuint m_model_view, m_projection;

	GLuint m_diffuse_material;
	GLuint m_normal_matrix;
	GLuint m_light_position;
	GLuint m_ambient_material;
	GLuint m_specular_material;
	GLuint m_shininess;

	mat4 m_mat_modelview, m_mat_projection;

	std::vector<Node> m_render_list;

}; // LightingShader

}

#endif // _E3D_LIGHTING_SHADER_H_