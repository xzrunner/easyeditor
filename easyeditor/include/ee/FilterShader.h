#ifndef _EASYEDITOR_FILTER_SHADER_H_
#define _EASYEDITOR_FILTER_SHADER_H_

#include "IShader.h"

namespace ee
{

struct ColorTrans;

class FilterShader : public IShader
{
public:
	FilterShader();
	virtual ~FilterShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetModelView(const Vector& offset, float scale);

	void SetMode(const std::string& mode);

	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);

protected:
	virtual void BindAttribLocation(GLuint prog) {}

	virtual void LoadShader() {}

}; // FilterShader

}

#endif // _EASYEDITOR_FILTER_SHADER_H_