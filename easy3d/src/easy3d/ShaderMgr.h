#ifndef _EASY3D_SHADER_MGR_H_
#define _EASY3D_SHADER_MGR_H_

#include <ee/ShaderMgrBase.h>
#include <ee/Matrix3D.h>
#include <ee/Color.h>

namespace e3d
{

class IModel;
class ModelShader;
class ShapeShader;
class SpriteShader;

class ShaderMgr : public ee::ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	virtual void NullProg();

	void Null();

	void Model();
	void Shape();
	void Sprite();

	void DrawModel(const IModel* model, const ee::mat4& m);
	void DrawShape(int type, const float* vertices, int count, 
		const ee::Colorf& col, bool force);
	void DrawShape(int type, const float* vertices, int count, 
		ee::Colorf* cols, bool force = true);
	void DrawShape(int type, const float* vertices, int vcount, const ee::Colorf& col,
		unsigned short* indices, int icount);
	void DrawTri(const ee::vec3 vertices[3], const ee::vec2 texcoords[3], int texid);
	void DrawTri(const float* vertices, const float* texcoords, int count, int texid);
	void DrawTriStrip(float* vertices, float* texcoords, int count, int texid);

	void SetModelView(const ee::mat4& mat);

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

#endif // _EASY3D_SHADER_MGR_H_