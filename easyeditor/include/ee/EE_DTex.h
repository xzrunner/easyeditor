#ifndef _EASYEDITOR_DTEX_H_
#define _EASYEDITOR_DTEX_H_

#include <string>
#include <map>

struct dtex_cg;

namespace ee
{

class Image;
class DTexC1;

class DTex
{
public:
	void LoadBegin();
	int Load(const Image* img);
	void LoadEnd();

	void ReloadBegin();
	void Reload(const Image* img);
	void ReloadEnd();

	float* Query(const Image* img, int* id);

	void Clear();

	dtex_cg* GetDtexCG();

	DTexC1* GetFbo0() { return m_fbo0; }
	DTexC1* GetFbo1() { return m_fbo1; }

	void OnSize(int w, int h);

	void DebugDraw() const;

	static DTex* Instance();

private:
	DTex();
	~DTex();

private:
	std::map<std::string, int> m_path2id;

	DTexC1 *m_fbo0, *m_fbo1;

private:
	static DTex* m_instance;

}; // DTex

}

#endif // _EASYEDITOR_DTEX_H_