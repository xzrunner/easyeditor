#ifndef _EASYPARTICLE3D_COCO_PACKER_H_
#define _EASYPARTICLE3D_COCO_PACKER_H_

#include <wx/wx.h>
#include <json/json.h>

namespace ebuilder { class CodeGenerator; }

namespace eparticle3d
{

class CocoPacker
{
public:
	static void PackDir(const wxString& srcdir, const wxString& dstfilename);

private:
	static void PackSingle(const wxString& filepath, ebuilder::CodeGenerator& gen);

	static void PackOldVersion(const Json::Value& val, ebuilder::CodeGenerator& gen, const std::string& dir);
	static void PackNewVersion(const Json::Value& val, ebuilder::CodeGenerator& gen, const std::string& dir);

}; // CocoPacker

}

#endif // _EASYPARTICLE3D_COCO_PACKER_H_
