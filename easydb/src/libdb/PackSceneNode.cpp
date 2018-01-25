#include "PackSceneNode.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <sns/NodeFactory.h>
#include <sns/NodeSym.h>
#include <sns/TexturePacker.h>
#include <memmgr/LinearAllocator.h>

#include <wx/arrstr.h>

#include <fstream>

namespace edb
{

std::string PackSceneNode::Command() const
{
	return "pack-scene-node";
}

std::string PackSceneNode::Description() const
{
	return "pack scene node";
}

std::string PackSceneNode::Usage() const
{
	std::string usage = Command() + " [src dir] [dst dir] [tp src dir] [tp filepath]";
	return usage;
}

int PackSceneNode::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[4])) return -1;

	Pack(argv[2], argv[3], argv[4], argv[5]);

	return 0;
}

void PackSceneNode::Pack(const std::string& src_dir, const std::string& dst_dir,
	                     const std::string& tp_src_dir, const std::string& tp_filepath)
{
	ee::FileHelper::MkDir(dst_dir, false);

	sns::TexturePacker tp(tp_src_dir);
	int idx = 1;
	while (true) {
		auto json_path = tp_filepath + std::to_string(idx) + ".json";
		if (ee::FileHelper::IsFileExist(json_path)) {
			tp.AddTexture(json_path);
			++idx;
		} else {
			break;
		}
	}

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (auto& file : files)
	{
		std::string filepath = file.ToStdString();
		auto ext = ee::FileHelper::GetExtension(filepath);
		if (ext != "json") {
			continue;
		}

		mm::LinearAllocator alloc;
		sns::NodeSym* sym = sns::NodeFactory::CreateSymFromJson(alloc, filepath);
		if (!sym) {
			continue;
		}
	
		uint8_t* data = nullptr;
		size_t len = 0;
		sym->StoreToBin(dst_dir, &data, len);

		auto relative_path = ee::FileHelper::GetRelativePath(src_dir, filepath);
		auto dst_path = ee::FileHelper::GetAbsolutePath(dst_dir, relative_path);
		dst_path = dst_path.substr(0, dst_path.find_last_of('.')) + ".bin";
		auto dst_dir = ee::FileHelper::GetFileDir(dst_path);
		ee::FileHelper::MkDir(dst_dir, false);

		std::ofstream fout(dst_path, std::ios::binary);
		fout.write(reinterpret_cast<const char*>(data), len);
		fout.close();
		delete data;
	}
}

}