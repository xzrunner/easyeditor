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
	static void pack(const wxString& srcdir, const wxString& dstfilename);

private:
	static void pack(const wxString& filepath, ebuilder::CodeGenerator& gen);

}; // CocoPacker

}

#endif // _EASYPARTICLE3D_COCO_PACKER_H_
