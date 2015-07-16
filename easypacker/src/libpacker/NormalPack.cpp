#include "NormalPack.h"
#include "MaxRectsBinaryPack2.h"
#include "ImageTrimData.h"

#include <easyimage.h>
#include <string>

namespace libpacker
{

NormalPack::NormalPack(const std::vector<std::string>& files)
	: m_filepaths(files)
	, m_extrude(1)
{
}

void NormalPack::Pack()
{
	for (int i = 0, n = m_filepaths.size(); i < n; ++i) {
		const wxString& path = m_filepaths[i];		
		RectSize sz;
		eimage::ImageIO::ReadHeader(path.mb_str(), sz.width, sz.height);
		sz.width += m_extrude * 2;
		sz.height += m_extrude * 2;
		m_src_sizes.push_back(sz);
	}

	MaxRectsBinaryPack2 packer;
	packer.Pack(libpacker::PACK_SQUARE_MULTI, m_src_sizes, m_dst_pos);
	packer.GetSize(m_dst_img_sz);

	m_dst_img_idx.resize(m_dst_img_sz.size());
	for (int i = 0, n = m_filepaths.size(); i < n; ++i) {
		const Rect& pos = m_dst_pos[i];
		m_dst_img_idx[pos.tex_id].push_back(i);
	}
}

void NormalPack::OutputInfo(const std::string& src_folder, const ImageTrimData& trim,
							const std::string& dst_file) const
{
	assert(m_filepaths.size() == m_src_sizes.size() && m_filepaths.size() == m_dst_pos.size());

	for (int i = 0, n = m_dst_img_idx.size(); i < n; ++i)
	{
		Json::Value value;
		for (int j = 0, m = m_dst_img_idx[i].size(); j < m; ++j) 
		{
			int idx = m_dst_img_idx[i][j];

			Json::Value frame_val;
			std::string filepath = d2d::FilenameTools::FormatFilepath(m_filepaths[idx]);	
			const ImageTrimData::Trim* t = trim.Query(filepath);
			if (!t) {
				throw d2d::Exception("NormalPack::OutputInfo didn't find trim info: %s\n", filepath);
			}

			frame_val["filename"] = filepath;

			const RectSize& src_sz = m_src_sizes[idx];

			const Rect& pos = m_dst_pos[idx];
			frame_val["frame"]["x"] = pos.x + m_extrude;
			frame_val["frame"]["y"] = pos.y + m_extrude;
			frame_val["frame"]["w"] = src_sz.width - m_extrude * 2;
			frame_val["frame"]["h"] = src_sz.height - m_extrude * 2;

			assert(src_sz.width == pos.width && src_sz.height == pos.height
				|| src_sz.width == pos.height && src_sz.height == pos.width);
			if (src_sz.width == pos.width && src_sz.height == pos.height) {
				frame_val["rotated"] = false;
			} else {
				frame_val["rotated"] = true;
			}

			frame_val["trimmed"] = false;
			frame_val["spriteSourceSize"]["x"] = t->x;
			frame_val["spriteSourceSize"]["y"] = t->y;
			frame_val["spriteSourceSize"]["w"] = t->w;
			frame_val["spriteSourceSize"]["h"] = t->h;

			frame_val["sourceSize"]["w"] = t->ori_w;
			frame_val["sourceSize"]["h"] = t->ori_h;

			value["frames"][j] = frame_val;
		}

		Json::Value meta_val;
		meta_val["size"]["w"] = m_dst_img_sz[i].width;
		meta_val["size"]["h"] = m_dst_img_sz[i].height;
		meta_val["scale"] = "1";
		meta_val["app"] = "easydb";

		value["meta"] = meta_val;

		std::string out_filepath = dst_file;
		out_filepath.insert(out_filepath.find_last_of("."), d2d::StringTools::IntToString(i + 1));

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(out_filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();		
	}
}

void NormalPack::OutputImage(const std::string& filepath) const
{
	assert(m_filepaths.size() == m_src_sizes.size() && m_filepaths.size() == m_dst_pos.size());

	for (int i = 0, n = m_dst_img_idx.size(); i < n; ++i) 
	{
		eimage::ImagePack pack(m_dst_img_sz[i].width, m_dst_img_sz[i].height);

		for (int j = 0, m = m_dst_img_idx[i].size(); j < m; ++j) 
		{
			int idx = m_dst_img_idx[i][j];
			const Rect& pos = m_dst_pos[idx];
			d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(m_filepaths[idx]);
			//		pack.AddImage(img, pos.x, pos.y, pos.width, pos.height, true, img->channels() == 4);
			pack.AddImage(img, pos.x + m_extrude, pos.y + m_extrude, 
				pos.width - m_extrude * 2, pos.height - m_extrude * 2, true, img->GetChannels() == 4, m_extrude);
			img->Release();
		}

		pack.PreMuiltiAlpha();

		std::string out_filepath = filepath;
		out_filepath.insert(out_filepath.find_last_of("."), d2d::StringTools::IntToString(i + 1));
		pack.OutputToFile(out_filepath);
	}
}

}