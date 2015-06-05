#include "ScreenFBO.h"

namespace d2d
{

ScreenFBO* ScreenFBO::m_instance = NULL;

ScreenFBO* ScreenFBO::Instance()
{
	if (!m_instance) {
		m_instance = new ScreenFBO();
	}
	return m_instance;
}

}