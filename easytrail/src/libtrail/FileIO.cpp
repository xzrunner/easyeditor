#include "FileIO.h"
#include "ToolbarPanel.h"
#include "MTConfigMgr.h"
#include "MotionTrail.h"
#include "LoadAdapter.h"
#include "mt_config.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>

#include <mt_2d.h>

#include <json/json.h>

#include <fstream>

namespace etrail
{

void FileIO::Store(const std::string& filepath, MotionTrail* mt, ToolbarPanel* toolbar)
{
	Json::Value value;
	std::string dir = ee::FileHelper::GetFileDir(filepath);
	toolbar->Store(value, dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const std::string& filepath, MotionTrail* mt, ToolbarPanel* toolbar, ee::LibraryPanel* library)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	toolbar->Load(value, dir);
}

MotionTrail* FileIO::LoadMT(const std::string& filepath)
{
	t2d_emitter_cfg* cfg = MTConfigMgr::Instance()->GetConfig(filepath);
	return new MotionTrail(cfg);
}

t2d_emitter_cfg* FileIO::LoadMTConfig(const std::string& filepath)
{
	LoadAdapter adapter;
	adapter.Load(filepath);

	int sz = SIZEOF_T2D_EMITTER_CFG + SIZEOF_T2D_SYMBOL * MAX_COMPONENTS;
	t2d_emitter_cfg* cfg = (t2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);

	cfg->count = adapter.count;

	cfg->life_begin = adapter.life_begin;
	cfg->life_offset = adapter.life_offset;

	cfg->fadeout_time = adapter.fadeout_time;

	cfg->mode_type = adapter.mode;

	if (adapter.mode == 0) {
		cfg->symbol_count = adapter.comp_images.size();
		cfg->symbols = (t2d_symbol*)(cfg+1);
		for (int i = 0; i < cfg->symbol_count; ++i) {
			const LoadAdapter::CompImage& src = adapter.comp_images[i];
			t2d_symbol& dst = cfg->symbols[i];

			memcpy(&dst.col_begin.r, &src.mul_col_begin.r, sizeof(dst.col_begin));
			memcpy(&dst.col_end.r, &src.mul_col_end.r, sizeof(dst.col_end));

			memcpy(&dst.mode.A.add_col_begin.r, &src.add_col_begin.r, sizeof(dst.mode.A.add_col_begin));
			memcpy(&dst.mode.A.add_col_end.r, &src.add_col_end.r, sizeof(dst.mode.A.add_col_end));

			dst.mode.A.scale_begin = src.scale_begin;
			dst.mode.A.scale_end = src.scale_end;

			dst.mode.A.ud = ee::SymbolMgr::Instance()->FetchSymbol(src.filepath);
			if (!dst.mode.A.ud) {
				throw ee::Exception("Symbol doesn't exist, [parent]:%s, [child]:%s !", filepath.c_str(), src.filepath.c_str());
			}
		}
	} else {
		assert(adapter.mode == 1);
		cfg->symbol_count = adapter.comp_shapes.size();
		cfg->symbols = (t2d_symbol*)(cfg+1);
		for (int i = 0; i < cfg->symbol_count; ++i) {
			const LoadAdapter::CompShape& src = adapter.comp_shapes[i];
			t2d_symbol& dst = cfg->symbols[i];

			memcpy(&dst.col_begin.r, &src.col_begin.r, sizeof(dst.col_begin));
			memcpy(&dst.col_end.r, &src.col_end.r, sizeof(dst.col_end));

			dst.mode.B.size = src.linewidth;
			dst.mode.B.acuity = src.acuity;
		}
	}

	return cfg;
}

}