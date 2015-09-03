#ifndef _LIBCOCO_COCO_PACKER_H_
#define _LIBCOCO_COCO_PACKER_H_

#include <drag2d.h>

namespace libcoco
{

class CocoPacker
{
public:
	CocoPacker(const std::string& json_dir, const std::string& tp_name, 
		const std::string& tp_dir, const std::string& outfile);
	
private:
	void LoadJsonData(const std::string& dir);
	void LoadTPData(const std::string& tp_name);

	void Pack(const std::string& outfile) const;

private:
	std::vector<const d2d::ISymbol*> m_symbols;

	d2d::TexturePacker m_tp;

}; // CocoPacker

}

#endif // _LIBCOCO_COCO_PACKER_H_