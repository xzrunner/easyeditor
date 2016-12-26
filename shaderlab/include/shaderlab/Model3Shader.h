#ifndef _SHADERLAB_MODEL3_SHADER_H_
#define _SHADERLAB_MODEL3_SHADER_H_

#include "Shader.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <unirender/VertexAttrib.h>

#include <vector>

#include <stdint.h>

struct ds_array;

namespace sl
{

namespace parser { class Shader; class Node; }

class RenderShader;
class RenderBuffer;
class ObserverMVP;
class ShaderProgram;

class Model3Shader : public Shader
{
public:
	Model3Shader(ur::RenderContext* rc);
	virtual ~Model3Shader();

	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

	void SetMaterial(const sm::vec3& ambient, const sm::vec3& diffuse, 
		const sm::vec3& specular, float shininess, int tex);
	void SetLightPosition(const sm::vec3& pos);

	void Draw(const ds_array* vertices, const ds_array* indices,
		bool has_normal, bool has_texcoord) const;

	// todo
	void SetModelView(const sm::mat4& mat);

private:
	void InitVAList();
	void InitProgs();

	void InitStaticColorProg(RenderBuffer* idx_buf);
	void InitGouraudShadingProg(RenderBuffer* idx_buf);
	void InitTextureMapProg(RenderBuffer* idx_buf);
	void InitGouraudTextureProg(RenderBuffer* idx_buf);

private:
	enum PROG_IDX {
		PI_STATIC_COLOR = 0,
		PI_GOURAUD_SHADING,
		PI_TEXTURE_MAP,
		PI_GOURAUD_TEXTURE,
		PROG_COUNT,
	};

	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		NORMAL,
		VA_MAX_COUNT
	};

	ShaderProgram* CreateProg(parser::Node* vert, parser::Node* frag, 
		const std::vector<VA_TYPE>& va_types, RenderBuffer* ib) const;

	struct GouraudUniforms
	{
		int diffuse, ambient, specular, shininess;
		int normal_matrix, light_position;

		void Init(RenderShader* shader);
		void SetMaterial(RenderShader* shader, const sm::vec3& ambient, const sm::vec3& diffuse, 
			const sm::vec3& specular, float shininess);
	};

private:
	ur::VertexAttrib m_va_list[VA_MAX_COUNT];

	ShaderProgram* m_programs[PROG_COUNT];

	GouraudUniforms m_shading_uniforms, m_texture_uniforms;

	mutable int m_curr_shader;

}; // Model3Shader

}

#endif // _SHADERLAB_MODEL3_SHADER_H_