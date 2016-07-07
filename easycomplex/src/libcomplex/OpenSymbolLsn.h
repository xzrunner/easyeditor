#ifndef _EASY_EASYCOMPLEX_OPEN_SYMBOL_LSN_H_
#define _EASY_EASYCOMPLEX_OPEN_SYMBOL_LSN_H_

namespace ecomplex
{

class OpenSymbolLsn
{
public:
	virtual ~OpenSymbolLsn() {}
	virtual void BeforeOpen() const = 0;
	virtual void AfterOpen() const = 0;
}; // OpenSymbolLsn

}

#endif // _EASY_EASYCOMPLEX_OPEN_SYMBOL_LSN_H_