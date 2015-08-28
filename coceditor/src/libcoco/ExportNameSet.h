#ifndef _LIBCOCO_EXPORT_NAME_SET_H_
#define _LIBCOCO_EXPORT_NAME_SET_H_

#include <drag2d.h>

namespace libcoco
{

class PackAnimation;

class ExportNameSet
{
public:
	
	void LoadExport(const d2d::ISymbol* symbol, PackAnimation* anim);

private:
	std::set<std::string> m_set;

}; // ExportNameSet

}

#endif // _LIBCOCO_EXPORT_NAME_SET_H_