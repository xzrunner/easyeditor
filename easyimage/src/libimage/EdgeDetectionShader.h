#ifndef _EASYIMAGE_EDGE_DETECTION_SHADER_H_
#define _EASYIMAGE_EDGE_DETECTION_SHADER_H_



namespace eimage
{

class EdgeDetectionShader : public ee::SpriteShader
{
public:
	EdgeDetectionShader();

	void SetBlend(float blend);

protected:
	virtual void LoadShader();

private:
	GLuint m_blend;

}; // EdgeDetectionShader

}

#endif // _EASYIMAGE_EDGE_DETECTION_SHADER_H_