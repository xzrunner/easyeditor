#ifndef _EASYPARTICLE2D_COCO_PACKER_H_
#define _EASYPARTICLE2D_COCO_PACKER_H_

#include <wx/wx.h>
#include <json/json.h>

namespace ebuilder { class CodeGenerator; }

namespace eparticle2d
{

class CocoPacker
{
public:
	static void pack(const std::string& srcdir, const std::string& dstfilename);

private:
	static void pack(const std::string& filepath, ebuilder::CodeGenerator& gen);
	static void pack(const Json::Value& val, ebuilder::CodeGenerator& gen, const std::string& dir);

//	static void PackFloat(const );

}; // CocoPacker

}

#endif // _EASYPARTICLE2D_COCO_PACKER_H_
