#ifndef _EASYEDITOR_DTEX_H_
#define _EASYEDITOR_DTEX_H_

#include <string>
#include <map>

struct dtex_cg;

namespace ee
{

class Image;

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

	void OnSize(int w, int h);

	void DebugDraw() const;

	static DTex* Instance();

private:
	DTex();

private:
	std::map<std::string, int> m_path2id;

private:
	static DTex* m_instance;

}; // DTex

}

#endif // _EASYEDITOR_DTEX_H_