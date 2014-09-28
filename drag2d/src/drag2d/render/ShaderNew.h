#ifndef D2D_SHADER_NEW_H
#define D2D_SHADER_NEW_H

namespace d2d
{

struct Colorf;
class Vector;
class ShaderImpl;

class ShaderNew
{
public:
	static ShaderNew* Instance();

	void SetSpriteColor(const Colorf& multi, const Colorf& add);
	void SetShapeColor(const Colorf& col);

	void sprite();
	void shape();
	void null();

	void release();

	void reload();

	int GetTexID() const;
	int GetFboID() const;

	void SetTexture(int tex);
	void SetFBO(int fbo);

	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);

	int GetVersion() const;

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetModelView(const Vector& offset, float scale);
	void SetProjection(int width, int height);

	// todo
	void Commit();

	void Flush();

private:
	ShaderNew();
	~ShaderNew();

private:
	static ShaderNew* m_instance;

private:
	ShaderImpl* m_impl;

}; // ShaderNew

}

#endif // D2D_SHADER_NEW_H