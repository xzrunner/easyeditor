#include "NormalPack.h"
#include "MaxRectsBinaryPack2.h"
#include "ImageTrimData.h"

#include <easyimage.h>
#include <string>

namespace libtexpacker
{

static const bool CLOCKWISE_ROT = true;

NormalPack::NormalPack(const std::vector<std::string>& files, const ImageTrimData* trim_info,
					   int extrude_min, int extrude_max, int start_id)
	: m_filepaths(files)
	, m_trim_info(trim_info)
	, m_extrude_min(extrude_min)
	, m_extrude_max(extrude_max)
	, m_start_id(start_id)
{
}

void NormalPack::Pack(int static_size, int max_size, int min_size)
{
	if (static_size <= 0) {
		Pack(PACK_SQUARE_MULTI_AUTO, -1, max_size, min_size);
	} else {
		Pack(PACK_SQUARE_MULTI, static_size);
	}
}

void NormalPack::OutputInfo(const std::string& dir, const std::string& dst_file) const
{
	assert(m_filepaths.size() == m_src_sizes.size() && m_filepaths.size() == m_dst_pos.size());

	for (int i = 0, n = m_dst_img_idx.size(); i < n; ++i)
	{
		Json::Value value;
		for (int j = 0, m = m_dst_img_idx[i].size(); j < m; ++j) 
		{
			int idx = m_dst_img_idx[i][j];

			Json::Value frame_val;

			//std::string filepath = d2d::FileHelper::FormatFilepath(m_filepaths[idx]);	
			std::string filepath = d2d::FileHelper::GetRelativePath(dir, m_filepaths[idx]);	
			frame_val["filename"] = filepath;

			const RectSize& src_sz = m_src_sizes[idx];

			const Rect& pos = m_dst_pos[idx];

			assert(src_sz.width == pos.width && src_sz.height == pos.height
				|| src_sz.width == pos.height && src_sz.height == pos.width);
			bool rot = (src_sz.width != pos.width || src_sz.height != pos.height);
			frame_val["rotated"] = rot;

			if (m_trim_info) 
			{
				const ImageTrimData::Trim* t = m_trim_info->Query(m_filepaths[idx]);
				if (!t) {
					throw d2d::Exception("NormalPack::OutputInfo didn't find trim_info info: %s\n", m_filepaths[idx]);
				}

				int e_left, e_right, e_bottom, e_up;
				GetExtrude(t->bound, t->w, t->h, e_left, e_right, e_bottom, e_up);

				frame_val["frame"]["w"] = src_sz.width - e_left - e_right;
				frame_val["frame"]["h"] = src_sz.height - e_bottom - e_up;
				if (!rot) {
					frame_val["frame"]["x"] = pos.x + e_left;
					frame_val["frame"]["y"] = pos.y + e_bottom;
				} else {
					if (CLOCKWISE_ROT) {
						frame_val["frame"]["x"] = pos.x + e_up;
						frame_val["frame"]["y"] = pos.y + e_left;
					} else {
						frame_val["frame"]["x"] = pos.x + e_bottom;
						frame_val["frame"]["y"] = pos.y + e_right;
					}
				}

				frame_val["trimmed"] = false;
				frame_val["spriteSourceSize"]["x"] = t->x;
				frame_val["spriteSourceSize"]["y"] = t->y;
				frame_val["spriteSourceSize"]["w"] = t->w;
				frame_val["spriteSourceSize"]["h"] = t->h;

				frame_val["sourceSize"]["w"] = t->ori_w;
				frame_val["sourceSize"]["h"] = t->ori_h;
			} 
			else 
			{	
				int e_left = m_extrude_min, 
					e_right = m_extrude_min, 
					e_bottom = m_extrude_min, 
					e_up = m_extrude_min;

				int w = src_sz.width - e_left - e_right, 
					h = src_sz.height - e_bottom - e_up;

				frame_val["frame"]["w"] = w;
				frame_val["frame"]["h"] = h;
				if (!rot) {
					frame_val["frame"]["x"] = pos.x + e_left;
					frame_val["frame"]["y"] = pos.y + e_bottom;
				} else {
					if (CLOCKWISE_ROT) {
						frame_val["frame"]["x"] = pos.x + e_up;
						frame_val["frame"]["y"] = pos.y + e_left;
					} else {
						frame_val["frame"]["x"] = pos.x + e_bottom;
						frame_val["frame"]["y"] = pos.y + e_right;
					}
				}

				frame_val["trimmed"] = false;
				frame_val["spriteSourceSize"]["x"] = 0;
				frame_val["spriteSourceSize"]["y"] = 0;
				frame_val["spriteSourceSize"]["w"] = w;
				frame_val["spriteSourceSize"]["h"] = h;

				frame_val["sourceSize"]["w"] = w;
				frame_val["sourceSize"]["h"] = h;
			}

			value["frames"][j] = frame_val;
		}

		Json::Value meta_val;
		meta_val["size"]["w"] = m_dst_img_sz[i].width;
		meta_val["size"]["h"] = m_dst_img_sz[i].height;
		meta_val["scale"] = "1";
		meta_val["app"] = "easydb";

		value["meta"] = meta_val;

		std::string out_filepath = dst_file;
		out_filepath.insert(out_filepath.find_last_of("."), d2d::StringHelper::ToString(m_start_id + i));

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
			const RectSize& src_sz = m_src_sizes[idx];

			bool rot;
			assert(src_sz.width == pos.width && src_sz.height == pos.height
				|| src_sz.width == pos.height && src_sz.height == pos.width);
			if (src_sz.width == pos.width && src_sz.height == pos.height) {
				rot = false;
			} else {
				rot = true;
			}

			d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(m_filepaths[idx]);

			int e_left, e_right, e_bottom, e_up;
			if (m_trim_info) {
				const ImageTrimData::Trim* t = m_trim_info->Query(m_filepaths[idx]);
				if (!t) {
					throw d2d::Exception("NormalPack::OutputInfo didn't find trim_info info: %s\n", m_filepaths[idx]);
				}

				assert(t->w == img->GetOriginWidth() && t->h == img->GetOriginHeight());
				GetExtrude(t->bound, t->w, t->h, e_left, e_right, e_bottom, e_up);
			} else {
				e_left = e_right = e_bottom = e_up = m_extrude_min;
			}
			
			if (rot) {
				if (CLOCKWISE_ROT) {
					pack.AddImage(img, pos.x + e_up, pos.y + e_left, pos.width - e_bottom - e_up, pos.height - e_left - e_right, 
						rot, CLOCKWISE_ROT, img->GetChannels() == 4, e_left, e_bottom, e_right, e_up);
				} else {
					pack.AddImage(img, pos.x + e_bottom, pos.y + e_right, pos.width - e_bottom - e_up, pos.height - e_left - e_right, 
						rot, CLOCKWISE_ROT, img->GetChannels() == 4, e_left, e_bottom, e_right, e_up);
				}
			} else {
				pack.AddImage(img, pos.x + e_left, pos.y + e_bottom, pos.width - e_left - e_right, pos.height - e_bottom - e_up, 
					rot, CLOCKWISE_ROT, img->GetChannels() == 4, e_left, e_bottom, e_right, e_up);
			}
			img->Release();
		}

		std::string out_filepath = filepath;
		out_filepath.insert(out_filepath.find_last_of("."), d2d::StringHelper::ToString(m_start_id + i));
		pack.OutputToFile(out_filepath);
	}
}

void NormalPack::Pack(PACK_STRATEGY strategy, int static_size, int max_size, int min_size)
{
	for (int i = 0, n = m_filepaths.size(); i < n; ++i) {
		const std::string& path = m_filepaths[i];		

		RectSize sz;
		d2d::LibpngAdapter::ReadHeader(path.c_str(), sz.width, sz.height);

		if (m_trim_info) {
			const ImageTrimData::Trim* t = m_trim_info->Query(m_filepaths[i]);
			if (!t) {
				throw d2d::Exception("NormalPack::OutputInfo didn't find trim_info info: %s\n", m_filepaths[i]);
			}
			int e_left, e_right, e_bottom, e_up;
			assert(t->w == sz.width && t->h == sz.height);
			GetExtrude(t->bound, t->w, t->h, e_left, e_right, e_bottom, e_up);

			sz.width += e_left + e_right;
			sz.height += e_bottom + e_up;
		} else {
			sz.width += m_extrude_min * 2;
			sz.height += m_extrude_min * 2;
		}

		m_src_sizes.push_back(sz);
	}

	MaxRectsBinaryPack2 packer;
	packer.Pack(strategy, static_size, max_size, min_size, m_src_sizes, m_dst_pos);
	packer.GetSize(m_dst_img_sz);

	m_dst_img_idx.resize(m_dst_img_sz.size());
	for (int i = 0, n = m_filepaths.size(); i < n; ++i) {
		const Rect& pos = m_dst_pos[i];
		m_dst_img_idx[pos.tex_id].push_back(i);
	}
}

void NormalPack::GetExtrude(const int bound[], int w, int h, int& left, int& right, int& bottom, int& up) const
{
	left = GetExtrude(bound[0], bound[1], h);
	up = GetExtrude(bound[2], bound[3], w);
	right = GetExtrude(bound[4], bound[5], h);
	bottom = GetExtrude(bound[6], bound[7], w);	
}

int NormalPack::GetExtrude(int max, int tot, int edge) const
{
	const float MAX_LIMIT = 25;
	const float TOT_LIMIT = 0.5f;
	if (max > MAX_LIMIT || (float)tot/edge > TOT_LIMIT) {
		return m_extrude_max;
	} else {
		return m_extrude_min;
	}
}

}