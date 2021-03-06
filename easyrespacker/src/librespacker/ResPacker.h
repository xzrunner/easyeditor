#ifndef _EASYRESPACKER_RES_PACKER_H_
#define _EASYRESPACKER_RES_PACKER_H_

#include <ee/TexturePacker.h>
#include <ee/Symbol.h>

#include <string>
#include <vector>

#include "typedef.h"
//#include "PackPkgMgr.h"

namespace erespacker
{

class ResPacker
{
public:
	ResPacker(const std::string& json_dir, const std::string& tp_name, 
		const std::string& tp_dir);
// 	ResPacker(const std::string& json_dir, const std::string& tp_name, 
// 		const std::string& tp_dir, const std::string& pkgs_cfg_file);
	~ResPacker();
	
	void OutputLua(const std::string& outfile, float scale = 1) const;

	void OutputEpe(const std::string& outfile, bool compress, float scale = 1) const;
	void OutputEpt(const std::string& outfile, int LOD = 0) const;

	void OutputUIExtra(const std::string& outfile) const;

	void OutputSprID(const std::string& outfile) const;

	void OutputTagKeyVal(const std::string& outfile) const;

	void OutputEptDesc(const std::string& outfile) const;

private:
	void LoadJsonData(const std::string& dir);
	void LoadTPData(const std::string& tp_name);

	void Pack() const;

	void AddUIWndSymbol(const std::string& filepath);

private:
	std::vector<ee::SymPtr> m_syms;

	ee::TexturePacker m_tp;

//	PackPkgMgr m_pkgs;

}; // ResPacker

}

#endif // _EASYRESPACKER_RES_PACKER_H_