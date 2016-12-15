#ifndef _EASYUI_QUERY_WINDOW_VIEW_SIZE_SJ_H_
#define _EASYUI_QUERY_WINDOW_VIEW_SIZE_SJ_H_

#include <ee/Subject.h>

namespace eui
{
namespace window
{

class QueryWindowViewSizeSJ : public ee::Subject
{
public:
	struct Params
	{
		int width;
		int height;
	};

	static const int DEFAULT_VIEW_WIDTH = 1136;
	static const int DEFAULT_VIEW_HEIGHT = 640;

public:
	void Query(int& width, int& height);

	static QueryWindowViewSizeSJ* Instance();

private:
	QueryWindowViewSizeSJ(int id);

private:
	static QueryWindowViewSizeSJ* m_instance;	

}; // QueryWindowViewSizeSJ

}
}

#endif // _EASYUI_QUERY_WINDOW_VIEW_SIZE_SJ_H_