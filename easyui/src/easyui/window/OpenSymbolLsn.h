#ifndef _EASYUI_WINDOW_OPEN_SYMBOL_LSN_H_
#define _EASYUI_WINDOW_OPEN_SYMBOL_LSN_H_

#include <easycomplex.h>

namespace eui
{
namespace window
{

class AnchorMgr;

class OpenSymbolLsn : public ecomplex::OpenSymbolLsn
{
public:
	OpenSymbolLsn(AnchorMgr* anchor_mgr) 
		: m_anchor_mgr(anchor_mgr) {}

	virtual void BeforeOpen() const;
	virtual void AfterOpen() const;

private:
	AnchorMgr* m_anchor_mgr;

}; // OpenSymbolLsn

}
}

#endif // _EASYUI_WINDOW_OPEN_SYMBOL_LSN_H_