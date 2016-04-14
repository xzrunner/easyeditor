#ifndef _EASYEDITOR_SHADER_MGR_H_
#define _EASYEDITOR_SHADER_MGR_H_

#include "Color.h"
#include "ShaderMgrBase.h"
#include "Vector3D.h"

namespace ee
{

class Vector;

class ShaderMgr : public ShaderMgrBase
{
public:
	enum Type
	{
		SHAPE = 0,
		SPRITE,
		BLEND,
		FILTER,
		SPRITE3,
		SHAPE3,

		COUNT,
	};

public:
	static ShaderMgr* Instance();

	virtual void NullProg();

	void SetShader(Type t);
	Type GetShader() const;
	IShader* GetShader(Type t) const;

	int GetTexID() const;
	int GetFboID() const;

	void SetTexture(int tex);
	void SetFBO(int fbo);

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetModelView(const Vector& offset, float scale);
	void SetModelView();	// sprite3

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	Type m_curr;

}; // ShaderMgr

}

#endif // _EASYEDITOR_SHADER_MGR_H_