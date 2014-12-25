#include "NormalPack.h"
#include "MaxRectsBinaryPack2.h"

#include <easyimage.h>

namespace libpacker
{

NormalPack::NormalPack(const std::vector<wxString>& files)
	: m_files(files)
{
}

void NormalPack::Pack()
{
	for (int i = 0, n = m_files.size(); i < n; ++i) {
		const wxString& path = m_files[i];		
		RectSize sz;
		eimage::ImageIO::ReadHeader(path.mb_str(), sz.width, sz.height);
		m_src_sizes.push_back(sz);
	}

	MaxRectsBinaryPack2 packer;
	packer.Pack(m_src_sizes, m_output);
	m_size = packer.GetSize();
}

void NormalPack::OutputInfo(const wxString& src_folder, const wxString& dst_file) const
{
	assert(m_files.size() == m_src_sizes.size() && m_files.size() == m_output.size());

	Json::Value value;
	for (int i = 0, n = m_files.size(); i < n; ++i) 
	{
		Json::Value frame_val;

		wxString filename = d2d::FilenameTools::getRelativePath(src_folder, m_files[i]);
		frame_val["filename"] = filename.ToStdString();

		const RectSize& src_sz = m_src_sizes[i];

		const Rect& pos = m_output[i];
		frame_val["frame"]["x"] = pos.x;
		frame_val["frame"]["y"] = pos.y;
		frame_val["frame"]["w"] = pos.width;
		frame_val["frame"]["h"] = pos.height;

		assert(src_sz.width == pos.width && src_sz.height == pos.height
			|| src_sz.width == pos.height && src_sz.height == pos.width);
		if (src_sz.width == pos.width && src_sz.height == pos.height) {
			frame_val["rotated"] = false;
		} else {
			frame_val["rotated"] = true;
		}

		frame_val["trimmed"] = false;
		frame_val["spriteSourceSize"]["x"] = 0;
		frame_val["spriteSourceSize"]["y"] = 0;
		frame_val["spriteSourceSize"]["w"] = pos.width;
		frame_val["spriteSourceSize"]["h"] = pos.height;

		frame_val["sourceSize"]["w"] = pos.width;
		frame_val["sourceSize"]["h"] = pos.height;

		value["frames"][i] = frame_val;
	}

	Json::Value meta_val;
	meta_val["size"]["w"] = m_size.width;
	meta_val["size"]["h"] = m_size.height;
	meta_val["scale"] = "1";

	value["meta"] = meta_val;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(dst_file.fn_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void NormalPack::OutputImage(const wxString& filepath) const
{
	assert(m_files.size() == m_src_sizes.size() && m_files.size() == m_output.size());

	eimage::ImagePack pack(m_size.width, m_size.height);
	for (int i = 0, n = m_files.size(); i < n; ++i) 
	{
		const Rect& pos = m_output[i];
		d2d::Image* img = d2d::ImageMgr::Instance()->getItem(m_files[i]);
		pack.AddImage(img, pos.x, pos.y, pos.width, pos.height);
		img->Release();
	}

	pack.OutputToFile(filepath);
}

}