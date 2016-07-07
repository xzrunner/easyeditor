#include "OpenSymbolLsn.h"
#include "AnchorMgr.h"

namespace eui
{
namespace window
{

void OpenSymbolLsn::BeforeOpen() const
{
	m_anchor_mgr->EnableObserve(false);
}

void OpenSymbolLsn::AfterOpen() const
{
	m_anchor_mgr->EnableObserve(true);
}

}
}