#include "ResetViewportSJ.h"
#include "message_id.h"

namespace eui
{
namespace window
{

ResetViewportSJ* ResetViewportSJ::m_instance = NULL;

ResetViewportSJ::ResetViewportSJ(int id)
	: Subject(id)
{
}

void ResetViewportSJ::Reset()
{
	Notify(NULL);
}

ResetViewportSJ* ResetViewportSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ResetViewportSJ(MSG_RESET_VIEWPORT);
	}
	return m_instance;
}

}
}