#ifndef _LIBCOCO_COCO_PACKER_H_
#define _LIBCOCO_COCO_PACKER_H_

#include <drag2d.h>

namespace libcoco
{

class CocoPacker
{
public:
	
private:
	void LoadData(const std::string& dir);

	void Pack() const;

private:
	std::vector<const d2d::ISymbol*> m_symbols;

}; // CocoPacker

}

#endif // _LIBCOCO_COCO_PACKER_H_