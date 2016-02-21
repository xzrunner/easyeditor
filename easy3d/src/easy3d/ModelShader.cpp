#include <gl/glew.h>

#include "ModelShader.h"
#include "IModel.h"
#include "Camera.h"

namespace e3d
{

static const int VERTEX_SIZE = 24;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_POSITION         = 0;
static const int ATTRIB_NORMAL	         = 1;
//static const int ATTRIB_DIFFUSE_MATERIAL = 2;

ModelShader::ModelShader()
{
}

void ModelShader::Load()
{
	LoadShader();

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");
}

void ModelShader::Unload()
{
}

void ModelShader::Bind()
{
	glUseProgram(m_prog);

	// todo
// 	glClearColor(0.5f, 0.5f, 0.5f, 1);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(ATTRIB_POSITION);
	glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(12));

//	glEnableVertexAttribArray(ATTRIB_DIFFUSE_MATERIAL);

	glEnable(GL_DEPTH_TEST);
}

void ModelShader::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(ATTRIB_POSITION);
	glDisableVertexAttribArray(ATTRIB_NORMAL);
//	glDisableVertexAttribArray(ATTRIB_DIFFUSE_MATERIAL);

	glDisable(GL_DEPTH_TEST);
}

void ModelShader::SetProjection(int width, int height)
{
	float hh = 1.0f * height / width;
	m_mat_projection = ee::mat4::Perspective(-1, 1, -hh, hh, 
		Camera::CAM_NEAR, Camera::CAM_FAR);
}

void ModelShader::Commit()
{
	if (m_render_list.empty()) {
		return;
	}

	//// Set the model-view transform.
	//glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.Pointer());

	// Set the projection transform.
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.Pointer());

	for (int i = 0, n = m_render_list.size(); i < n; ++i) 
	{
		const Node& node = m_render_list[i];

		// Set the model-view transform.
		ee::mat4 model_view = node.mat * m_mat_modelview;
		glUniformMatrix4fv(m_model_view, 1, 0, model_view.Pointer());

		// Set the normal matrix.
		// It's orthogonal, so its Inverse-Transpose is itself!
 	 	ee::mat3 normal_mat = model_view.ToMat3();
		SetNormalMatrix(normal_mat);

		const std::vector<Mesh>& meshes = node.model->GetAllMeshes();
		for (int j = 0, m = meshes.size(); j < m; ++j) 
		{
			const Mesh& mesh = meshes[j];

			SetMaterial(mesh.material);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer);
			glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));
			glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(12));
			//glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(12));
			//glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);

			glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_SHORT, 0);
		}
	}

	m_render_list.clear();
}

void ModelShader::SetModelView(const ee::mat4& mat)
{
	m_mat_modelview = mat;
}

void ModelShader::Draw(const IModel* model, const ee::mat4& m)
{
	Node n;
	n.model = model;
	n.mat = m;
	m_render_list.push_back(n);
}

void ModelShader::BindAttrib(GLuint prog)
{
	glBindAttribLocation(prog, ATTRIB_POSITION, "position");
	glBindAttribLocation(prog, ATTRIB_NORMAL, "normal");
//	glBindAttribLocation(prog, ATTRIB_DIFFUSE_MATERIAL, "diffuse_material");
}

}