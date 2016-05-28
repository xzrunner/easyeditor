#ifndef _EASYEDITOR_DTEX_C1_H_
#define _EASYEDITOR_DTEX_C1_H_

struct dtex_c1;

namespace ee
{

class DTexC1
{
public:
	DTexC1(int tex_size);
	~DTexC1();

	void Bind();
	void Unbind();

	int GetTextureId();
	int GetTextureSize();

	void Clear(float xmin, float ymin, float xmax, float ymax);

	void DebugDraw() const;

private:
	dtex_c1* m_c1;

}; // DTexC1

}

#endif // _EASYEDITOR_DTEX_C1_H_