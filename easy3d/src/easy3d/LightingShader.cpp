#define NOMINMAX 
#include <gl/GLee.h>

#include "LightingShader.h"
#include "IModel.h"

namespace e3d
{

LightingShader::LightingShader()
{
}

void LightingShader::Bind()
{
	ModelShader::Bind();

	// Set the light position.
	vec4 lightPosition(0.25, 0.25, 1, 0);
	glUniform3fv(m_light_position, 1, lightPosition.Pointer());

	//// Set up some default material parameters.
	// 	glUniform3f(m_ambient_material, 0.04f, 0.04f, 0.04f);
	// 	glUniform3f(m_specular_material, 0.5, 0.5, 0.5);
	// 	glUniform1f(m_shininess, 50);
}

void LightingShader::LoadShader()
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

	InitShader(vs, fs);

	m_diffuse_material = glGetUniformLocation(m_prog, "diffuse_material");
	m_normal_matrix = glGetUniformLocation(m_prog, "u_normal_matrix");
	m_light_position = glGetUniformLocation(m_prog, "u_light_position");
	m_ambient_material = glGetUniformLocation(m_prog, "u_ambient_material");
	m_specular_material = glGetUniformLocation(m_prog, "u_specular_material");
	m_shininess = glGetUniformLocation(m_prog, "u_shininess");
}

void LightingShader::SetNormalMatrix(const mat3& noraml_mat)
{
	glUniformMatrix3fv(m_normal_matrix, 1, 0, noraml_mat.Pointer());
}

void LightingShader::SetMaterial(const Material& material)
{
	//glVertexAttrib4f(
	//	ATTRIB_DIFFUSE_MATERIAL, 
	//	material.diffuse.x, 
	//	material.diffuse.y, 
	//	material.diffuse.z, 
	//	1);

	glUniform3f(
		m_diffuse_material, 
		material.diffuse.x, 
		material.diffuse.y, 
		material.diffuse.z);
	glUniform3f(
		m_ambient_material, 
		material.ambient.x, 
		material.ambient.y, 
		material.ambient.z);
	glUniform3f(
		m_specular_material, 
		material.specular.x, 
		material.specular.y, 
		material.specular.z);

	glUniform1f(m_shininess, 50);
}

}