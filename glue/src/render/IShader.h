#ifndef _GLUE_ISHADER_H_
#define _GLUE_ISHADER_H_

namespace glue
{
	
class IShader
{
public:
	IShader() {}
	virtual ~IShader() {}
	
	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void OnSize(int width, int height) = 0;

	virtual void Commit() = 0;

	virtual void Reset() = 0;

}; // IShader

}

#endif // _DRAG2D_ISHADER_H_