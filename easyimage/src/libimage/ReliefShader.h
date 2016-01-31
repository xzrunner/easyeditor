#ifndef _EASYIMAGE_RELIEF_SHADER_H_
#define _EASYIMAGE_RELIEF_SHADER_H_



namespace eimage
{

class ReliefShader : public ee::SpriteShader
{
public:
	ReliefShader();

protected:
	virtual void LoadShader();

}; // ReliefShader

}

#endif // _EASYIMAGE_RELIEF_SHADER_H_