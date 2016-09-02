#include "FileIO.h"
#include "ToolBarPanel.h"
#include "ComponentPanel.h"
#include "ParticleSystem.h"
#include "PSConfigMgr.h"
#include "ps_config.h"

#include <ps_2d.h>

#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/Math2D.h>
#include <ee/SymbolMgr.h>
#include <ee/JsonSerializer.h>

#include <json/json.h>

#include <string>
#include <fstream>

namespace eparticle2d
{

void FileIO::Store(const std::string& filepath, ToolbarPanel* toolbar)
{
	Json::Value value;

	toolbar->Store(value);

//	value["name"] = toolbar->m_name->GetValue().ToStdString();

	value["mode_type"] = toolbar->m_mode_choice->GetSelection();

	value["rotation_is_dir"] = toolbar->m_rotation_is_dir_ctrl->GetValue();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = toolbar->m_children.size(); i < n; ++i)
	{
		ComponentPanel* cp = toolbar->m_children[i];
		p2d_symbol* p_symbol = cp->m_pc;

		Json::Value cval;

		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(p_symbol->ud));
		cval["filepath"] = ee::FileHelper::GetRelativePath(dir, sym->GetFilepath());

		for (int j = 0, m = cp->m_sliders.size(); j < m; ++j) {
			cp->m_sliders[j]->Store(cval);
		}

		ee::JsonSerializer::Store(p_symbol->mul_col_begin.rgba, cval["mul_col_begin"]);
		ee::JsonSerializer::Store(p_symbol->mul_col_end.rgba, cval["mul_col_end"]);
		ee::JsonSerializer::Store(p_symbol->add_col_begin.rgba, cval["add_col_begin"]);
		ee::JsonSerializer::Store(p_symbol->add_col_end.rgba, cval["add_col_end"]);

		value["components"][i] = cval;
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const std::string& filepath, ParticleSystem* ps, ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	toolbar->Load(value);

	LoadAdapter adapter;
	adapter.Load(filepath);

//	toolbar->m_name->SetValue(adapter.name);

	toolbar->m_mode_choice->SetSelection(adapter.mode_type);
	ps->SetMode(adapter.mode_type);

	toolbar->m_rotation_is_dir_ctrl->SetValue(adapter.A.rotation_is_dir);
	ps->SetRotationIsDir(adapter.A.rotation_is_dir);

	for (size_t i = 0, n = adapter.components.size(); i < n; ++i) {
		toolbar->Add(adapter.components[i]);
	}
}

ParticleSystem* FileIO::LoadPS(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	p2d_emitter_cfg* cfg = PSConfigMgr::Instance()->GetConfig(filepath);
	return new ParticleSystem(cfg);
}

p2d_emitter_cfg* FileIO::LoadPSConfig(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	LoadAdapter adapter;
	adapter.Load(filepath);

	int sz = SIZEOF_P2D_EMITTER_CFG + SIZEOF_P2D_SYMBOL * MAX_COMPONENTS;
	p2d_emitter_cfg* cfg = (p2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);

	cfg->mode_type = adapter.mode_type;

	if (cfg->mode_type == P2D_MODE_GRAVITY)
	{
		cfg->mode.A.gravity.x = adapter.A.gravity.x;
		cfg->mode.A.gravity.y = adapter.A.gravity.y;

		cfg->mode.A.speed = adapter.A.speed;
		cfg->mode.A.speed_var = adapter.A.speed_var;

		cfg->mode.A.tangential_accel = adapter.A.tangential_accel;
		cfg->mode.A.tangential_accel_var = adapter.A.tangential_accel_var;
		cfg->mode.A.radial_accel = adapter.A.radial_accel;
		cfg->mode.A.radial_accel_var = adapter.A.radial_accel_var;

		cfg->mode.A.rotation_is_dir = adapter.A.rotation_is_dir;
	} 
	else if (cfg->mode_type == P2D_MODE_RADIUS)
	{
		cfg->mode.B.start_radius = adapter.B.start_radius;
		cfg->mode.B.start_radius_var = adapter.B.start_radius_var;
		cfg->mode.B.end_radius = adapter.B.end_radius;
		cfg->mode.B.end_radius_var = adapter.B.end_radius_var;

		cfg->mode.B.direction_delta = adapter.B.direction_delta;
		cfg->mode.B.direction_delta_var = adapter.B.direction_delta_var;
	}
	else if (cfg->mode_type == P2D_MODE_SPD_COS)
	{
		cfg->mode.C.speed = adapter.C.speed;
		cfg->mode.C.speed_var = adapter.C.speed_var;

		cfg->mode.C.cos_amplitude = adapter.C.cos_amplitude;
		cfg->mode.C.cos_amplitude_var = adapter.C.cos_amplitude_var;
		cfg->mode.C.cos_frequency = adapter.C.cos_frequency;
		cfg->mode.C.cos_frequency_var = adapter.C.cos_frequency_var;
	}

	cfg->emission_time = adapter.emission_time;
	cfg->count = adapter.count;

	cfg->life = adapter.life;
	cfg->life_var = adapter.life_var;

	cfg->position.x = adapter.position.x;
	cfg->position.y = adapter.position.y;
	cfg->position_var.x = adapter.position_var.x;
	cfg->position_var.y = adapter.position_var.y;

	cfg->direction = adapter.direction;
	cfg->direction_var = adapter.direction_var;

	cfg->sym_count = adapter.components.size();
	cfg->syms = (p2d_symbol*)(cfg+1);
	for (int i = 0, n = adapter.components.size(); i < n; ++i) 
	{
		const LoadAdapter::Component& src = adapter.components[i];
		p2d_symbol& dst = cfg->syms[i];

		dst.angle_start = src.angle_start * SM_DEG_TO_RAD;
		dst.angle_end = src.angle_end * SM_DEG_TO_RAD;

		dst.scale_start = src.scale_start * 0.01f;
		dst.scale_end = src.scale_end * 0.01f;

		memcpy(&dst.mul_col_begin.r, &src.mul_col_begin.r, sizeof(src.mul_col_begin));
		memcpy(&dst.mul_col_end.r, &src.mul_col_end.r, sizeof(src.mul_col_end));
		memcpy(&dst.add_col_begin.r, &src.add_col_begin.r, sizeof(src.add_col_begin));
		memcpy(&dst.add_col_end.r, &src.add_col_end.r, sizeof(src.add_col_end));

		dst.ud = static_cast<s2::Symbol*>(ee::SymbolMgr::Instance()->FetchSymbol(src.filepath));
	}

	return cfg;
}

}