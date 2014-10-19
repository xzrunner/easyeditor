#include "Matrix.h"

#define NOMINMAX 
#include <gl/GLee.h>

#include "LightingShader.h"
#include "ISurface.h"
#include "IModel.h"

namespace e3d
{

static const int VERTEX_SIZE = 24;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_POSITION         = 0;
static const int ATTRIB_NORMAL	         = 1;
//static const int ATTRIB_DIFFUSE_MATERIAL = 2;

LightingShader::LightingShader()
{

}

LightingShader::~LightingShader()
{

}

void LightingShader::Load()
{
	static const char* vs =
		FLOAT_PRECISION
		"attribute vec4 position;  \n"
		"attribute vec3 normal;  \n"
//		"attribute vec3 diffuse_material;  \n"
		"uniform vec3 diffuse_material;  \n"

		"varying vec4 v_dest_color;  \n"

		"uniform mat3 u_normal_matrix;  \n"
		"uniform vec3 u_light_position;  \n"
		"uniform vec3 u_ambient_material;  \n"
		"uniform vec3 u_specular_material;  \n"
		"uniform float u_shininess;  \n"

		"uniform mat4 u_projection;  \n"
		"uniform mat4 u_modelview;  \n"

		"void main(void)  \n"
		"{  \n"
		"	vec3 N = u_normal_matrix * normal;  \n"
		"	vec3 L = normalize(u_light_position);  \n"
		"	vec3 E = vec3(0, 0, 1);  \n"
		"	vec3 H = normalize(L + E);  \n"

		"	float df = max(0.0, dot(N, L));  \n"
		"	float sf = max(0.0, dot(N, H));  \n"
		"	sf = pow(sf, u_shininess);  \n"

		"	vec3 color = u_ambient_material + df * diffuse_material + sf * u_specular_material;  \n"
		"	v_dest_color = vec4(color, 1);  \n"
		"	gl_Position = u_projection * u_modelview * position;  \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying vec4 v_dest_color;  \n"

		"void main(void)  \n"
		"{  \n"
		"	gl_FragColor = v_dest_color;  \n"
		"}  \n"
		;

	InitShader(fs, vs);

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	m_diffuse_material = glGetUniformLocation(m_prog, "diffuse_material");
	m_normal_matrix = glGetUniformLocation(m_prog, "u_normal_matrix");
	m_light_position = glGetUniformLocation(m_prog, "u_light_position");
	m_ambient_material = glGetUniformLocation(m_prog, "u_ambient_material");
	m_specular_material = glGetUniformLocation(m_prog, "u_specular_material");
	m_shininess = glGetUniformLocation(m_prog, "u_shininess");

	// Set up some default material parameters.
// 	glUniform3f(m_ambient_material, 0.04f, 0.04f, 0.04f);
// 	glUniform3f(m_specular_material, 0.5, 0.5, 0.5);
// 	glUniform1f(m_shininess, 50);
}

void LightingShader::Unload()
{
	
}

void LightingShader::Bind()
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

void LightingShader::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(ATTRIB_POSITION);
	glDisableVertexAttribArray(ATTRIB_NORMAL);
//	glDisableVertexAttribArray(ATTRIB_DIFFUSE_MATERIAL);

	glDisable(GL_DEPTH_TEST);
}

void LightingShader::BindAttrib(GLuint prog)
{
	glBindAttribLocation(prog, ATTRIB_POSITION, "position");
	glBindAttribLocation(prog, ATTRIB_NORMAL, "normal");
//	glBindAttribLocation(prog, ATTRIB_DIFFUSE_MATERIAL, "diffuse_material");
}

void LightingShader::Commit()
{
	if (m_render_list.empty()) {
		return;
	}

	// Set the light position.
	vec4 lightPosition(0.25, 0.25, 1, 0);
	glUniform3fv(m_light_position, 1, lightPosition.Pointer());

	//// Set the model-view transform.
	//glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.Pointer());

	// Set the projection transform.
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.Pointer());

	// Set the normal matrix.
	// It's orthogonal, so its Inverse-Transpose is itself!
	mat3 normalMatrix = m_mat_modelview.ToMat3();
	glUniformMatrix3fv(m_normal_matrix, 1, 0, normalMatrix.Pointer());

	for (int i = 0, n = m_render_list.size(); i < n; ++i) 
	{
		const Node& node = m_render_list[i];

		// Set the model-view transform.
//		mat4 model_view = m_mat_modelview * node.ori.ToMatrix() * mat4::Translate(node.pos.x, node.pos.y, node.pos.z);
		mat4 model_view = mat4(node.ori.ToMatrix()) * mat4::Translate(node.pos.x, node.pos.y, node.pos.z) * m_mat_modelview;

		glUniformMatrix4fv(m_model_view, 1, 0, model_view.Pointer());

		const std::vector<Mesh>& meshes = node.model->GetAllMeshes();
		for (int j = 0, m = meshes.size(); j < m; ++j) 
		{
			const Mesh& mesh = meshes[j];

//  			glVertexAttrib4f(
//  				ATTRIB_DIFFUSE_MATERIAL, 
//  				mesh.material.diffuse.x, 
//  				mesh.material.diffuse.y, 
//  				mesh.material.diffuse.z, 
//  				1);

 			glUniform3f(
 				m_diffuse_material, 
 				mesh.material.diffuse.x, 
 				mesh.material.diffuse.y, 
 				mesh.material.diffuse.z);
			glUniform3f(
				m_ambient_material, 
				mesh.material.ambient.x, 
				mesh.material.ambient.y, 
				mesh.material.ambient.z);
			glUniform3f(
				m_specular_material, 
				mesh.material.specular.x, 
				mesh.material.specular.y, 
				mesh.material.specular.z);

			glUniform1f(m_shininess, 50);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer);
			glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));
			glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(12));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);

			glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_SHORT, 0);
		}
	}

	m_render_list.clear();
}

void LightingShader::SetModelView(const mat4& mat)
{
	m_mat_modelview = mat;
}

void LightingShader::SetProjection(int width, int height)
{
// 	float h = 4.0f * height / width;
// 	h = 3;
// 	m_mat_projection = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 12);

	float hh = 1.0f * height / width;
	m_mat_projection = mat4::Frustum(-1, 1, -hh, hh, 1000, 1010);
}

void LightingShader::Draw(const IModel* model, const vec3& pos,
						  const Quaternion& ori)
{
	Node n;
	n.model = model;
	n.pos = pos;
	n.ori = ori;
	m_render_list.push_back(n);
}

}