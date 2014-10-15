#ifndef _D2D_SHADER_MGR_H_
#define _D2D_SHADER_MGR_H_

#include "ShaderMgrBase.h"

namespace d2d
{

struct Colorf;
class Vector;
class ShaderImpl;
class IShader;

class ShaderMgr : public ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	void SetSpriteColor(const Colorf& multi, const Colorf& add);
	void SetShapeColor(const Colorf& col);

	void sprite();
	void shape();
	void null();

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
	void GetModelView(Vector& offset, float& scale);

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	ShaderImpl* m_impl;

}; // ShaderMgr

}

#endif // _D2D_SHADER_MGR_H_