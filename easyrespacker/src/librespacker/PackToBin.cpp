#include "PackToBin.h"
#include "PackNodeFactory.h"
#include "IPackNode.h"
#include "tools.h"
#include "spritepack.h"

#include <epbin.h>

namespace librespacker
{

void PackToBin::PackEPE(const std::string& filepath, 
						const d2d::TexturePacker& tp, 
						bool compress)
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	const std::map<std::string, int>& export_set = factory->GetExportSet();

	std::vector<IPackNode*> nodes;
	factory->GetAllNodes(nodes);
	if (nodes.empty()) {
		return;
	}
	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());

	std::vector<std::string> textures;
	tp.GetAllTextureFilename(textures);

	// header
	uint16_t export_n = export_set.size();
	uint16_t maxid = nodes[nodes.size() - 1]->GetID();
	uint16_t tex = textures.size();
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
		nodes[i]->PackToBin(&ptr, tp);
	}

	// write file
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
	if (compress) {
		uint8_t* dst = NULL;
		size_t dst_sz;
		epbin::Lzma::Compress(&dst, &dst_sz, buf, out_sz);

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(dst_sz));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
	} else {
		int32_t sz = -(int)out_sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(out_sz));
		fout.write(reinterpret_cast<const char*>(buf), out_sz);
	}
	delete[] buf;
	fout.close();
}

void PackToBin::PackEPT(const std::string& filepath, const d2d::TexturePacker& tp, 
						TextureType type, int LOD)
{
	std::string ext;
	switch (type) 
	{
	case TT_PNG4: case TT_PNG8:
		ext = ".png";
		break;
	case TT_PVR:
		ext = ".pvr";
		break;
	case TT_PKM:
		ext = ".pkm";
		break;
	}

	std::vector<std::string> filenames;
	tp.GetAllTextureFilename(filenames);
	for (int i = 0, n = filenames.size(); i < n; ++i) 
	{
		std::locale::global(std::locale(""));
		std::string _filepath = filepath + "." + d2d::StringTools::ToString(i + 1) + ".ept";
		std::ofstream fout(_filepath.c_str(), std::ios::binary);
		std::locale::global(std::locale("C"));

		std::string str = filenames[i];
		str = str.substr(0, str.find_last_of('.')) + ext;

		epbin::TexLoader* loader;
		switch (type)
		{
		case TT_PNG4:
			loader = new epbin::PNGLoader(false);
			break;
		case TT_PNG8:
			loader = new epbin::PNGLoader(true);
			break;
		case TT_PVR:
			loader = new epbin::PVRLoader();
			break;
		case TT_PKM:
			loader = new epbin::PKMLoader();
			break;
		default:
			throw d2d::Exception("PackToBin::PackEPT unknown type: %d\n", type);
		}

		float scale = 1;
		for (int i = 0; i < LOD; ++i) {
			loader->Load(str);
			loader->Store(fout);
			scale *= 0.5f;
		}

		delete loader;

		fout.close();
	}
}

}