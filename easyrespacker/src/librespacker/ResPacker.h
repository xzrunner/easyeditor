#ifndef _LIBRESPACKER_RES_PACKER_H_
#define _LIBRESPACKER_RES_PACKER_H_

#include <drag2d.h>

#include "typedef.h"

namespace librespacker
{

class ResPacker
{
public:
	ResPacker(const std::string& json_dir, const std::string& tp_name, 
		const std::string& tp_dir);
	
	void OutputLua(const std::string& outfile) const;

	void OutputEpe(const std::string& outfile, bool compress) const;
	void OutputEpt(const std::string& outfile, TextureType type, int LOD) const;

private:
	void LoadJsonData(const std::string& dir);
	void LoadTPData(const std::string& tp_name);

	void Pack() const;

private:
	std::vector<const d2d::ISymbol*> m_symbols;

	d2d::TexturePacker m_tp;

}; // ResPacker

}

#endif // _LIBRESPACKER_RES_PACKER_H_