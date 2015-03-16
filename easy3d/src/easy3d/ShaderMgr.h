#ifndef _E3D_SHADER_MGR_H_
#define _E3D_SHADER_MGR_H_

#include <drag2d.h>

#include "Matrix.h"

namespace e3d
{

class IModel;
class ModelShader;
class ShapeShader;
class SpriteShader;

class ShaderMgr : public d2d::ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	void Null();
	void Model();
	void Shape();
	void Sprite();

	void DrawModel(const IModel* model, const mat4& m);
	void DrawShape(int type, float* vertices, int count, 
		const d2d::Colorf& col, bool force);
	void DrawShape(int type, float* vertices, int count, 
		d2d::Colorf* cols, bool force = true);
	void DrawShape(int type, float* vertices, int vcount, const d2d::Colorf& col,
		unsigned short* indices, int icount);
	void DrawTri(const vec3 vertices[3], const vec2 texcoords[3], int texid);
	void DrawTri(float* vertices, float* texcoords, int count, int texid);
	void DrawTriStrip(float* vertices, float* texcoords, int count, int texid);

	void SetModelView(const mat4& mat);

	void SetModelShader(ModelShader* shader, bool delete_old = true);

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	ModelShader* m_model_shader;
	ShapeShader* m_shape_shader;
	SpriteShader* m_sprite_shader;

}; // ShaderMgr

}

#endif // _E3D_SHADER_MGR_H_