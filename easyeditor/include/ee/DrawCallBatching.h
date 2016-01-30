#ifndef _EASYEDITOR_DRAW_CALL_BATCHING_H_
#define _EASYEDITOR_DRAW_CALL_BATCHING_H_

#include <string>
#include <map>

struct dtex_cg;

namespace ee
{

class Image;

class DrawCallBatching
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

	static DrawCallBatching* Instance();

private:
	DrawCallBatching();
	~DrawCallBatching();

private:
	std::map<std::string, int> m_path2id;

private:
	static DrawCallBatching* m_instance;

}; // DrawCallBatching

}

#endif // _EASYEDITOR_DRAW_CALL_BATCHING_H_