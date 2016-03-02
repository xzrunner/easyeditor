//#ifndef _EASYEDITOR_BLEND_SHADER_H_
//#define _EASYEDITOR_BLEND_SHADER_H_
//
//#include "SpriteShader.h"
//#include "BlendModes.h"
//
//namespace ee
//{
//
//class BlendShader : public SpriteShader
//{
//public:
//	BlendShader();
//
//	virtual void Bind();
//	virtual void Unbind();
//
//	void SetBaseTexID(int tex);
//
//	void SetBlendMode(const std::string& mode);
//
//	void DrawBlend(const Vector vertices[4], const Vector texcoords[4], 
//		const Vector texcoords_base[4], int texid);
//
//protected:
//	virtual void BindAttribLocation(GLuint prog);
//	virtual void GetUniformLocation(GLuint prog);
//
//	virtual void LoadShader();
//
//private:
//	void CopyVertex(const float vb[24]);
//
//private:
//	BlendMode m_mode;
//
//	typedef int GLint;
//	GLint m_mode_id;
//	GLint m_sampler0, m_sampler1;
//
//}; // BlendShader
//
//}
//
//#endif // _EASYEDITOR_BLEND_SHADER_H_