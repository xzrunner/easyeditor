#ifndef _EASYEDITOR_BLEND_SHADER_H_
#define _EASYEDITOR_BLEND_SHADER_H_

#include "IShader.h"

namespace ee
{

struct ColorTrans;

class BlendShader : public IShader
{
public:
	BlendShader();
	virtual ~BlendShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetModelView(const Vector& offset, float scale);

	void SetBlendMode(const std::string& mode);

	void SetColor(const ColorTrans& color);

	void Draw(const Vector vertices[4], const Vector texcoords[4], 
		const Vector texcoords_base[4], int texid, int texid_base);

protected:
	virtual void BindAttribLocation(GLuint prog) {}

	virtual void LoadShader() {}

}; // BlendShader

}

#endif // _EASYEDITOR_BLEND_SHADER_H_