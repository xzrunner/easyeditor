#include "PackToBin.h"
#include "PackNodeFactory.h"
#include "IPackNode.h"
#include "pack_unpack.h"
#include "Lzma.h"

#include "PackPNG.h"
#include "PackPVR.h"
#include "PackETC1.h"
#include "PackETC2.h"

#include <ee/TexturePacker.h>
#include <ee/StringHelper.h>

#include <timp/TextureFormat.h>

#include "spritepack.h"

#include <algorithm>

namespace erespacker
{

void PackToBin::PackEPE(const std::string& filepath, 
						const ee::TexturePacker& tp, 
						bool compress, float scale)
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	const std::map<std::string, int>& export_set = factory->GetExportSet();

	std::vector<IPackNode*> nodes;
	factory->GetAllNodes(nodes);
	if (nodes.empty()) {
		return;
	}
	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());

	// header
	uint16_t export_n = export_set.size();
	uint16_t maxid = nodes[nodes.size() - 1]->GetSprID();
	uint16_t tex = tp.GetTextures().size();
	uint32_t unpack_sz = 0;
	unpack_sz += SIZEOF_PACK + tex * sizeof(int);
	int align_n = (maxid + 1 + 3) & ~3;
	unpack_sz += align_n * sizeof(uint8_t);
	unpack_sz += (maxid + 1) * SIZEOF_POINTER;
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		unpack_sz += nodes[i]->SizeOfUnpackFromBin();
	}

	// body
	uint32_t body_sz = 0;
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		body_sz += nodes[i]->SizeOfPackToBin();
	}

	// cal size
	int32_t out_sz = 0;
	out_sz += sizeof(export_n) + sizeof(maxid) + sizeof(tex) + sizeof(unpack_sz);
	out_sz += sizeof(body_sz);
	std::map<std::string, int>::const_iterator itr = export_set.begin();
	for ( ; itr != export_set.end(); ++itr) {
		out_sz += sizeof(uint16_t);		// id
		out_sz += sizeof_pack_str(itr->first);
	}
	out_sz += body_sz;

	// filling data
	uint8_t* buf = new uint8_t[out_sz];
	uint8_t* ptr = buf;
	pack(export_n, &ptr);
	pack(maxid, &ptr);
	pack(tex, &ptr);
	pack(unpack_sz, &ptr);
	pack(body_sz, &ptr);
	for (itr = export_set.begin(); itr != export_set.end(); ++itr) {
		uint16_t id = itr->second;
		pack(id, &ptr);
		pack_str(itr->first, &ptr);
	}

	for (int i = 0, n = nodes.size(); i < n; ++i) {
		nodes[i]->PackToBin(&ptr, tp, scale);
	}

	// write file
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	if (compress) {
		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, out_sz);
		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(dst_sz));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
		}
	} else {
		int32_t sz = -(int)out_sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(out_sz));
		fout.write(reinterpret_cast<const char*>(buf), out_sz);
	}
	delete[] buf;
	fout.close();
}

void PackToBin::PackEPT(const std::string& filepath, const ee::TexturePacker& tp, int LOD)
{
	const std::vector<const ee::TexturePacker::Texture*>& textures = tp.GetTextures();
	for (int i = 0, n = textures.size(); i < n; ++i) 
	{
		const ee::TexturePacker::Texture* tex = textures[i];

		std::string img_path = tex->filepath.substr(0, tex->filepath.find_last_of('.'));
		PackImage* packer = NULL;
		if (tex->format == "png") {
			packer = new PackPNG(true);
			img_path += ".png";
		} else if (tex->format == "pvr") {
			packer = new PackPVR(true);
			img_path += ".pvr";
		} else if (tex->format == "etc1") {
			packer = new PackETC1();
		} else if (tex->format == "etc2") {
			packer = new PackETC2(true);
			img_path += ".pkm";
		} else {
			throw ee::Exception("PackToBin::PackEPT unknown type: %s\n", tex->format.c_str());
		}

		packer->Load(img_path);

		float pack_scale = 1.0f;
		for (int lod = 0; lod <= LOD; ++lod) 
		{
			std::string idx_str = "." + ee::StringHelper::ToString(i + 1);
			std::string scale_str = "";
			if (LOD != 0 && pack_scale != 1) {
				scale_str = "." + ee::StringHelper::ToString((int)(pack_scale * 100));
			}
			std::string fmt_str = ".ept";

			std::string out_filepath = filepath + idx_str + scale_str + fmt_str;
			packer->Store(out_filepath, pack_scale);
			pack_scale *= 0.5f;
		}

		delete packer;
	}
}

void PackToBin::PackEPT(const std::string& src_file, const std::string& dst_file, 
						int type)
{
	std::string ext;
	switch (type) 
	{
	case timp::TEXTURE_RGBA4: case timp::TEXTURE_RGBA8:
		ext = ".png";
		break;
	case timp::TEXTURE_PVR2: case timp::TEXTURE_PVR4:
		ext = ".pvr";
		break;
	case timp::TEXTURE_ETC1: case timp::TEXTURE_ETC2:
		ext = ".pkm";
		break;
	}

	PackImage* packer;
	switch (type)
	{
	case timp::TEXTURE_RGBA4:
		packer = new PackPNG(false);
		break;
	case timp::TEXTURE_RGBA8:
		packer = new PackPNG(true);
		break;
	case timp::TEXTURE_PVR2: case timp::TEXTURE_PVR4:
		packer = new PackPVR(true);
		break;
	case timp::TEXTURE_ETC1:
		packer = new PackETC1();
		break;
	case timp::TEXTURE_ETC2:
		packer = new PackETC2(true);
		break;
	default:
		throw ee::Exception("PackToBin::PackEPT unknown type: %d\n", type);
	}

	packer->Load(src_file);
	packer->Store(dst_file, 1);

	delete packer;
}

}