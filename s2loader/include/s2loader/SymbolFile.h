#ifndef _S2LOADER_SYMBOL_FILE_H_
#define _S2LOADER_SYMBOL_FILE_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2loader
{

class SymbolFile
{
public:
	int Type(const CU_STR& filepath) const;

	const CU_STR& Tag(int type) const;

public:
	CU_STR UNKNOWN_TAG;

private:
	void Regist(int type, const CU_STR& tag);

private:
	CU_MAP<int, CU_STR> m_type2tag;
	CU_MAP<CU_STR, int> m_tag2type;

	CU_SINGLETON_DECLARATION(SymbolFile);

}; // SymbolFile

}

#endif // _S2LOADER_SYMBOL_FILE_H_
