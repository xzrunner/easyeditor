#include "DataMgr.h"

namespace eanim
{

DataMgr* DataMgr::m_instance = NULL;

DataMgr* DataMgr::Instance()
{
	if (!m_instance) {
		m_instance = new DataMgr;
	}
	return m_instance;
}

}