#ifndef _EASYUI_RESET_VIEWPORT_SJ_H_
#define _EASYUI_RESET_VIEWPORT_SJ_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class ResetViewportSJ : public d2d::Subject
{
public:
	void Reset();

	static ResetViewportSJ* Instance();

private:
	ResetViewportSJ(int id);

private:
	static ResetViewportSJ* m_instance;	

}; // ResetViewportSJ

}
}

#endif // _EASYUI_RESET_VIEWPORT_SJ_H_