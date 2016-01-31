#ifndef _EASYEJOY2D_SCREEN_H_
#define _EASYEJOY2D_SCREEN_H_



typedef unsigned int GLuint;

namespace eejoy2d
{

class EJScreen
{
public:
	void Load();
	void Unload();

	void Bind();
	void UnBind();

	int GetWidth() const;
	int GetHeight() const;
	GLuint GetTexID() const;

	static void OnSize(int w, int h);

	void Clear();

	void DebugDraw() const;

public:
	static EJScreen* Instance();
	static EJScreen* Create();

private:
	EJScreen() {}
	~EJScreen() {}

	void CreateRes();
	void ReleaseRes();

private:
	GLuint m_tex, m_fbo;

private:
	static EJScreen* m_instance;

	static bool m_loaded;

}; // EJScreen

}

#endif // _EASYEJOY2D_SCREEN_H_