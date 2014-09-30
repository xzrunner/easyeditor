#ifndef D2D_SHADER_NEW_H
#define D2D_SHADER_NEW_H

#include "sketch/sketch3d.h"

namespace d2d
{

struct Colorf;
class Vector;
class ShaderImpl;
class IShader;

class ShaderNew
{
public:
	static ShaderNew* Instance();

	void SetSpriteColor(const Colorf& multi, const Colorf& add);
	void SetShapeColor(const Colorf& col);

	void sprite();
	void shape();
	void lighting();
	void null();

	void release();

	void reload();

	int GetTexID() const;
	int GetFboID() const;

	void SetTexture(int tex);
	void SetFBO(int fbo);

	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);
	// todo
	void Draw(const z3d::IModel* model);

	int GetVersion() const;

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetModelView(const Vector& offset, float scale);
	void GetModelView(Vector& offset, float& scale);
	void SetProjection(int width, int height);

	// todo
	void Commit();

	void Flush();

private:
	ShaderNew();
	~ShaderNew();

	void Switch(IShader* shader);

private:
	static ShaderNew* m_instance;

private:
	ShaderImpl* m_impl;

}; // ShaderNew

}

#endif // D2D_SHADER_NEW_H