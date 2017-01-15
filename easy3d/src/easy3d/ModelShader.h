#ifndef _EASY3D_MODEL_SHADER_H_
#define _EASY3D_MODEL_SHADER_H_

#include <vector>

namespace e3d
{

class IModel;
struct Material;

class ModelShader
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

	void SetModelView(const sm::mat4& mat);

	void Draw(const IModel* model, const sm::mat4& m);

protected:
	virtual void BindAttrib(GLuint prog);

	virtual void LoadShader() = 0;

	virtual void SetNormalMatrix(const sm::mat3& noraml_mat) {}
	virtual void SetMaterial(const Material& material) {}

private:
	struct Node
	{
		const IModel* model;
		sm::mat4 mat;
	};

private:
	GLuint m_model_view, m_projection;

	sm::mat4 m_mat_modelview, m_mat_projection;

	std::vector<Node> m_render_list;

}; // ModelShader

}

#endif // _EASY3D_MODEL_SHADER_H_