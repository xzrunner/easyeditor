#ifndef _EASYUI_CHANGE_WINDOW_VIEW_SIZE_SJ_H_
#define _EASYUI_CHANGE_WINDOW_VIEW_SIZE_SJ_H_



namespace eui
{
namespace window
{

class ChangeWindowViewSizeSJ : public ee::Subject
{
public:
	struct Params
	{
		int width;
		int height;
	};

public:
	void Change(int width, int height);

	static ChangeWindowViewSizeSJ* Instance();

private:
	ChangeWindowViewSizeSJ(int id);

private:
	static ChangeWindowViewSizeSJ* m_instance;	

}; // ChangeWindowViewSizeSJ

}
}

#endif // _EASYUI_CHANGE_WINDOW_VIEW_SIZE_SJ_H_