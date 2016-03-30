#ifndef _EASYEDITOR_SPRITE_SHADER_H_
#define _EASYEDITOR_SPRITE_SHADER_H_

#include "IShader.h"

namespace ee
{

class RenderColor;

class SpriteShader : public IShader
{
public:
	SpriteShader();
	virtual ~SpriteShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset();

	void SetModelView(const Vector& offset, float scale);

// 	int GetTexID() const;
// 	void SetTexID(int tex);
// 
// 	int GetFboID() const;
// 	void SetFboID(int fbo);

// 	void SetBufferData(bool open);
// 	bool IsOpenBufferData() const;

	void SetColor(const RenderColor& color);

//	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);

protected:
	virtual void BindAttribLocation(GLuint prog) {}

	virtual void LoadShader() {}

}; // SpriteShader

}

#endif // _EASYEDITOR_SPRITE_SHADER_H_