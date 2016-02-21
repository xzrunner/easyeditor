#include "FileIO.h"
#include "LoadAdapter.h"
#include "ParticleSystem.h"
#include "PSConfigMgr.h"
#include "ToolBarPanel.h"
#include "ComponentPanel.h"
#include "ps_config.h"
#include "config.h"

#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/SliderCtrl.h>
#include <ee/Math2D.h>
#include <ee/SymbolMgr.h>

#include <ps_3d.h>

#include <wx/spinctrl.h>

#include <fstream>

namespace eparticle3d
{

void FileIO::Store(const std::string& filepath, ParticleSystem* ps,
				   ToolbarPanel* toolbar)
{
	Json::Value value;

	toolbar->Store(value);
	value["version"] = VERSION;

//	value["name"] = toolbar->m_name->GetValue();

	value["min_hori"] = toolbar->m_min_hori->GetValue();
	value["max_hori"] = toolbar->m_max_hori->GetValue();
	value["min_vert"] = toolbar->m_min_vert->GetValue();
	value["max_vert"] = toolbar->m_max_vert->GetValue();

	value["ground"] = toolbar->m_ground->GetSelection();

	value["orient_to_movement"] = ps->GetEmitter()->cfg->orient_to_movement;

	value["loop"] = ps->GetEmitter()->loop;

//	value["orient_to_parent"] = toolbar->m_orient_to_parent->GetValue();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (size_t i = 0, n = toolbar->m_children.size(); i < n; ++i)
	{
		ComponentPanel* cp = toolbar->m_children[i];
		p3d_symbol* pc = cp->m_pc;

		ee::Symbol* symbol = static_cast<ee::Symbol*>(pc->ud);
		value["components"][i]["filepath"] = 
			ee::FileHelper::GetRelativePath(dir, symbol->GetFilepath());

		if (pc->bind_ps_cfg) {
			std::string filepath = PSConfigMgr::Instance()->GetFilepath(pc->bind_ps_cfg);
			value["components"][i]["bind ps filepath"] = 
				ee::FileHelper::GetRelativePath(dir, filepath);
		}

		value["components"][i]["name"] = cp->m_name->GetValue().ToStdString();
		for (int j = 0, m = cp->m_sliders.size(); j < m; ++j) {
			cp->m_sliders[j]->Store(value["components"][i]);
		}

		Json::Value col_val;
		col_val["r"] = pc->col_mul.r;
		col_val["g"] = pc->col_mul.g;
		col_val["b"] = pc->col_mul.b;
		value["components"][i]["mul_col"] = col_val;

		col_val["r"] = pc->col_add.r;
		col_val["g"] = pc->col_add.g;
		col_val["b"] = pc->col_add.b;
		value["components"][i]["add_col"] = col_val;
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const std::string& filepath, ParticleSystem* ps,
				  ToolbarPanel* toolbar, ee::LibraryPanel* library)
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

	int version = value["version"].asInt();
	if (version == 0) {
		value["life"]["center"] = adapter.life;
		value["life"]["offset"] = adapter.life_var;
	}

	if (value["radial_speed"].isNull()) {
		value["radial_speed"]["center"] = adapter.radial_spd * 4;
		value["radial_speed"]["offset"] = adapter.radial_spd_var * 4;
	}

	toolbar->Load(value, version);

//	toolbar->m_name->SetValue(adapter.name);

	toolbar->m_min_hori->SetValue((adapter.hori - adapter.hori_var) * ee::TRANS_RAD_TO_DEG);
	toolbar->m_max_hori->SetValue((adapter.hori + adapter.hori_var) * ee::TRANS_RAD_TO_DEG);
	ps->SetHori(toolbar->m_min_hori->GetValue(), toolbar->m_max_hori->GetValue());

	toolbar->m_min_vert->SetValue((adapter.vert - adapter.vert_var) * ee::TRANS_RAD_TO_DEG);
	toolbar->m_max_vert->SetValue((adapter.vert + adapter.vert_var) * ee::TRANS_RAD_TO_DEG);
	ps->SetVert(toolbar->m_min_vert->GetValue(), toolbar->m_max_vert->GetValue());

	toolbar->m_ground->SetSelection(adapter.ground);
	ps->SetGround(adapter.ground);

	toolbar->m_orient_to_movement->SetValue(adapter.orient_to_movement);
	ps->SetOrientToMovement(adapter.orient_to_movement);

//	toolbar->m_orient_to_parent->SetValue(adapter.orient_to_parent);

	for (size_t i = 0, n = adapter.components.size(); i < n; ++i) {
		toolbar->Add(adapter.components[i], library);
	}
}

ParticleSystem* FileIO::LoadPS(const std::string& filepath)
{
	p3d_emitter_cfg* cfg = PSConfigMgr::Instance()->GetConfig(filepath);
	return new ParticleSystem(cfg, false);
}

p3d_emitter_cfg* FileIO::LoadPSConfig(const std::string& filepath)
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
	
	int sz = SIZEOF_P3D_EMITTER_CFG + SIZEOF_P3D_SYMBOL * MAX_COMPONENTS;
	p3d_emitter_cfg* cfg = (p3d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);
	
	cfg->emission_time = adapter.emission_time;
	cfg->count = adapter.count;

	cfg->life = adapter.life * 0.001f;
	cfg->life_var = adapter.life_var * 0.001f;

	cfg->hori = adapter.hori;
	cfg->hori_var = adapter.hori_var;
	cfg->vert = adapter.vert;
	cfg->vert_var = adapter.vert_var;

	cfg->radial_spd = adapter.radial_spd;
	cfg->radial_spd_var = adapter.radial_spd_var;
	cfg->tangential_spd = adapter.tangential_spd;
	cfg->tangential_spd_var = adapter.tangential_spd_var;
	cfg->angular_spd = adapter.angular_spd;
	cfg->angular_spd_var = adapter.angular_spd_var;

	cfg->dis_region = adapter.dis_region;
	cfg->dis_region_var = adapter.dis_region_var;
	cfg->dis_spd = adapter.dis_spd;
	cfg->dis_spd_var = adapter.dis_spd_var;

	cfg->gravity = adapter.gravity;

	cfg->linear_acc = adapter.linear_acc;
	cfg->linear_acc_var = adapter.linear_acc_var;

	cfg->fadeout_time = adapter.fadeout_time;

	cfg->ground = adapter.ground;

	cfg->start_radius = adapter.start_radius;
	cfg->start_height = adapter.start_height;

	cfg->orient_to_movement = adapter.orient_to_movement;

	// todo dir
	cfg->dir.x = 0;
	cfg->dir.y = 0;
	cfg->dir.z = 1;

	cfg->symbol_count = adapter.components.size();
	cfg->symbols = (p3d_symbol*)(cfg+1);
	for (int i = 0, n = adapter.components.size(); i < n; ++i) {
		const LoadAdapter::Component& src = adapter.components[i];
		p3d_symbol& dst = cfg->symbols[i];

		dst.scale_start = src.scale_start * 0.01f;
		dst.scale_end = src.scale_end * 0.01f;

		dst.angle = src.angle;
		dst.angle_var = src.angle_var;

		memcpy(&dst.col_mul.r, &src.col_mul.r, sizeof(src.col_mul));
		memcpy(&dst.col_add.r, &src.col_add.r, sizeof(src.col_add));
		dst.alpha_start = src.alpha_start * 0.01f;
		dst.alpha_end = src.alpha_end * 0.01f;

		if (ee::FileHelper::IsFileExist(src.bind_filepath)) {
			dst.bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(src.bind_filepath);
		}

		dst.ud = ee::SymbolMgr::Instance()->FetchSymbol(src.filepath);
	}

	return cfg;
}

}