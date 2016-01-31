#ifndef _EASYIMAGE_BLUR_SHADER_H_
#define _EASYIMAGE_BLUR_SHADER_H_



namespace eimage
{

class BlurShader : public ee::SpriteShader
{
public:
	BlurShader();

	void SetRadius(float r);

protected:
	virtual void LoadShader();

private:
	GLuint m_radius;

}; // BlurShader

}

#endif // _EASYIMAGE_BLUR_SHADER_H_