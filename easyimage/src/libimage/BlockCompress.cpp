#include "BlockCompress.h"

#include <drag2d.h>

namespace eimage
{

const int BlockCompress::TOLERANCE = 50;

BlockCompress::BlockCompress(const std::vector<std::string>& image_files)
	: m_image_files(image_files)
{
	m_tot_area = 0;
	m_tot_block = m_used_block = m_saved_block = 0;
}

BlockCompress::~BlockCompress()
{
	for_each(m_blocks.begin(), m_blocks.end(), DeletePointerFunctor<Block>());
	for_each(m_debug_pic.begin(), m_debug_pic.end(), DeletePointerFunctor<Picture>());
}

void BlockCompress::Compress()
{
	for (int i = 0; i < m_image_files.size(); ++i) {
		printf("Compress [%d/%d]\n", i, m_image_files.size());
		Compress(m_image_files[i]);

		PrintStatistics();
	}

	PrintStatistics();



	std::ofstream fout("block_compress.txt");
	float used = m_used_block * 16;
	fout << used / m_tot_area << " " << used / (m_tot_block * 16) << std::endl;
	fout.close();
}

void BlockCompress::Uncompress(const std::string& dir) const
{
	for (int i = 0; i < m_debug_pic.size(); ++i) {
		Uncompress(dir, *m_debug_pic[i]);
	}
}

void BlockCompress::Compress(const std::string& filepath)
{
	int w, h, c, f;
	uint8_t* pixels = d2d::LibpngAdapter::Read(filepath.c_str(), w, h, c, f);
	m_tot_area += w * h;

	Picture* pic = new Picture;
	pic->w = w;
	pic->h = h;
	pic->filepath = filepath;

	for (int y = 0; y * 4 < h; ++y) {
		for (int x = 0; x * 4 < w; ++x) {
			if (IsBlockTransparent(pixels, w, h, x, y)) {
				pic->blocks.push_back(NULL);
				continue;
			}

			++m_tot_block;

			Block* b = new Block(pixels, w, h, x, y);
			Block* same = NULL;
			for (int i = 0; i < m_blocks.size(); ++i) {
				if (b->IsSame(m_blocks[i])) {
					same = m_blocks[i];
					break;
				}
			}
			if (same) {
				++m_saved_block;
				delete b;
				pic->blocks.push_back(same);
			} else {
				++m_used_block;
				m_blocks.push_back(b);
				pic->blocks.push_back(b);
			}
		}
	}

	m_debug_pic.push_back(pic);
}

void BlockCompress::Uncompress(const std::string& dir, const Picture& pic) const
{
	int sz = pic.w * pic.h * 4;
	uint8_t* pixels = new uint8_t[sz];
	memset(pixels, 0, sz);

	int idx = 0;
	for (int y = 0; y * 4 < pic.h; ++y) {
		for (int x = 0; x * 4 < pic.w; ++x) {
			Block* b = pic.blocks[idx];
			if (b) {
				b->CopyToPicture(pic, x, y, pixels);
			}
			++idx;
		}
	}

//	std::string filepath = dir + "//" + d2d::FilenameTools::getFilenameWithExtension(pic.filepath);
	std::string filepath = pic.filepath;
	d2d::LibpngAdapter::Write(pixels, pic.w, pic.h, filepath.c_str());

	delete[] pixels;
}

void BlockCompress::PrintStatistics() const
{
	printf("tolerance:%d\n", TOLERANCE);
	printf("tot_area:%d\n", m_tot_area);
	printf("tot_block:%d, used:%d, saved:%d\n", m_tot_block, m_used_block, m_saved_block);

	float used = m_used_block * 16;
	printf("after percent:%0.2f %0.2f\n", used / m_tot_area, used / (m_tot_block * 16));
}

bool BlockCompress::IsBlockTransparent(uint8_t* pixels, int w, int h, int x, int y)
{
	for (int px = x * 4; px < x * 4 + 4; ++px) {
		for (int py = y * 4; py < y * 4 + 4; ++py) {
			if (px >= w || py >= h) {
				continue;
			}
			if (pixels[(w * py + px) * 4 + 4 - 1] != 0) {
				return false;
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// class BlockCompress::Block
//////////////////////////////////////////////////////////////////////////

BlockCompress::Block::
Block(uint8_t* pixels, int w, int h, int x, int y)
{
	m_pixels = new uint8_t[4 * 4 * 4];
	uint8_t* ptr = m_pixels;
	for (int py = y * 4; py < y * 4 + 4; ++py) {
		for (int px = x * 4; px < x * 4 + 4; ++px) {
			if (px >= w || py >= h) {
				memset(ptr, 0, 4);				
			} else {
				int src_idx = (w * py + px) * 4;
				memcpy(ptr, &pixels[src_idx], sizeof(uint8_t) * 4);
			}
			ptr += 4;
		}
	}
}

BlockCompress::Block::
~Block()
{
	delete m_pixels;
}

bool BlockCompress::Block::
IsSame(const Block* b) const
{
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			int dis_sqr = PixelDistanceSquare(b, x, y);
			if (dis_sqr > TOLERANCE) {
				return false;
			}
		}
	}
	return true;
}

void BlockCompress::Block::
CopyToPicture(const Picture& pic, int x, int y, uint8_t* pixels) const
{
	for (int py = y * 4; py < y * 4 + 4; ++py) {
		for (int px = x * 4; px < x * 4 + 4; ++px) {
			if (px >= pic.w || py >= pic.h) {
				continue;
			}
			int src_ptr = ((py - y * 4) * 4 + (px - x * 4)) * 4;
			int dst_ptr = ((pic.h - 1 - py) * pic.w + px) * 4;
			memcpy(&pixels[dst_ptr], &m_pixels[src_ptr], sizeof(uint8_t) * 4);
		}
	}
}

int BlockCompress::Block::
PixelDistanceSquare(const Block* b, int x, int y) const
{
	assert(x >= 0 && x < 4 && y >= 0 && y < 4);
	int dis = 0;
	int idx = (4 * y + x) * 4;
	for (int i = idx; i < idx + 4; ++i) {
		int len = m_pixels[i] - b->m_pixels[i];
		dis += len * len;
	}
	return dis;
}

}