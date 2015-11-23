#ifndef _DRAG2D_DRAW_CALL_BATCHING_H_
#define _DRAG2D_DRAW_CALL_BATCHING_H_

#include <string>
#include <map>

namespace d2d
{

class Image;

class DrawCallBatching
{
public:
	void Begin();
	int Add(const Image* img);
	void End();

	float* Query(const Image* img, int* id);
	
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

#endif // _DRAG2D_DRAW_CALL_BATCHING_H_