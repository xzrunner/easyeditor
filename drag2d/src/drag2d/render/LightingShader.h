#ifndef _DRAG2D_LIGHTING_SHADER_H_
#define _DRAG2D_LIGHTING_SHADER_H_

#include "IShader.h"

#include "sketch/Matrix.h"
#include "sketch/Quaternion.h"
#include "sketch/sketch3d.h"

namespace d2d
{

class LightingShader : public IShader
{
public:
	LightingShader();
	virtual ~LightingShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void Commit();

	virtual void SetModelView(const Vector& offset, float scale) {}
	virtual void SetModelView(const Quaternion& ori);
	virtual void SetProjection(int width, int height);

	void Draw(const z3d::IModel* model, const vec3& pos);

protected:
	virtual void BindAttrib(GLuint prog);

private:
	struct Node
	{
		const z3d::IModel* model;
		vec3 pos;
	};

private:
	GLuint m_model_view, m_projection;

	GLuint m_diffuse_material;
	GLuint m_normal_matrix;
	GLuint m_light_position;
	GLuint m_ambient_material;
	GLuint m_specular_material;
	GLuint m_shininess;

	mat4 m_translation;
	mat4 m_mat_modelview, m_mat_projection;

//	std::vector<const z3d::IModel*> m_models;
	std::vector<Node> m_render_list;

}; // LightingShader

}

#endif // _DRAG2D_LIGHTING_SHADER_H_