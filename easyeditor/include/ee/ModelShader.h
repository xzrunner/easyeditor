#ifndef _EASYEDITOR_MODEL_SHADER_H_
#define _EASYEDITOR_MODEL_SHADER_H_

#include "IShader.h"
#include "Vector3D.h"

#include <vector>

namespace ee
{

class Vector;
struct ColorTrans;

class ModelShader : public IShader
{
public:
	ModelShader();
	virtual ~ModelShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetModelView(const Vector& offset, float scale);

	void SetColor(const ColorTrans& color);

	void Draw(const std::vector<vec3>& positions, const std::vector<Vector>& texcoords, int texid);

public:
	static const int FAR = 50;
	static const int NEAR = 2;

protected:
	virtual void BindAttribLocation(GLuint prog) {}

	virtual void LoadShader() {}

}; // ModelShader

}

#endif // _EASYEDITOR_MODEL_SHADER_H_