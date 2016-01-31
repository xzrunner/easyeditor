#ifndef _E3D_MODEL_SHADER_H_
#define _E3D_MODEL_SHADER_H_

#include "Matrix.h"



namespace e3d
{

class IModel;
struct Material;

class ModelShader : public ee::IShader
{
public:
	ModelShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset() {}

	void SetModelView(const mat4& mat);

	void Draw(const IModel* model, const mat4& m);

protected:
	virtual void BindAttrib(GLuint prog);

	virtual void LoadShader() = 0;

	virtual void SetNormalMatrix(const mat3& noraml_mat) {}
	virtual void SetMaterial(const Material& material) {}

private:
	struct Node
	{
		const IModel* model;
		mat4 mat;
	};

private:
	GLuint m_model_view, m_projection;

	mat4 m_mat_modelview, m_mat_projection;

	std::vector<Node> m_render_list;

}; // ModelShader

}

#endif // _E3D_MODEL_SHADER_H_