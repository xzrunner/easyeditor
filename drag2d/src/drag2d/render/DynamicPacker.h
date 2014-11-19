#ifndef _DRAG2D_DYNAMIC_PACKER_H_
#define _DRAG2D_DYNAMIC_PACKER_H_

namespace d2d
{

typedef unsigned int GLuint;

class TPNode;

class DynamicPacker
{
public:
	DynamicPacker(int width, int height, int padding, int extrude);
	virtual ~DynamicPacker();

	int GetTextureID() const { return m_tex; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	int GetExtend() const { return m_padding + m_extrude; }

	void ReloadTexture();

	void DebugDrawStatic() const;
	void DebugDraw() const;

protected:
	virtual void ReloadPixels() = 0;

	void InitTexture(int tex_id = 0);
	void InitFBO(int fbo_id = 0);
	void InitRoot();

	void ClearTexture();

protected:
	int m_width, m_height;
	int m_padding;
	int m_extrude;

	GLuint m_tex;
	GLuint m_fbo;

	TPNode* m_root;

}; // DynamicPacker

}

#endif // _DRAG2D_DYNAMIC_PACKER_H_