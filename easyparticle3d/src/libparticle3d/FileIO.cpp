#include "FileIO.h"
#include "FileAdapter.h"
#include "ParticleSystem.h"
#include "PSConfigMgr.h"
#include "ToolBarPanel.h"
#include "ps_config.h"
#include "config.h"

#include <ps/particle3d.h>

namespace eparticle3d
{

void FileIO::Store(const std::string& filepath, ToolbarPanel* toolbar)
{
	Json::Value value;

	toolbar->Store(value);
	value["version"] = VERSION;

	value["name"] = toolbar->m_name->GetValue().ToStdString();
	value["package"] = toolbar->m_package->GetValue().ToStdString();

	value["layer"] = toolbar->m_layer->GetValue();

	value["min_hori"] = toolbar->m_min_hori->GetValue();
	value["max_hori"] = toolbar->m_max_hori->GetValue();
	value["min_vert"] = toolbar->m_min_vert->GetValue();
	value["max_vert"] = toolbar->m_max_vert->GetValue();

	value["inertia"] = toolbar->m_inertia->GetValue();

	value["bounce"] = toolbar->m_bounce->GetValue();

	value["additive_blend"] = toolbar->m_additiveBlend->GetValue();

	value["start_radius_3d"] = toolbar->m_radius_3d->GetValue();

	value["orient_to_movement"] = toolbar->m_orient_to_movement->GetValue();

	value["orient_to_parent"] = toolbar->m_orient_to_parent->GetValue();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	for (size_t i = 0, n = toolbar->m_children.size(); i < n; ++i)
	{
		ToolbarPanel::ChildPanel* cp = toolbar->m_children[i];
		particle_symbol* pc = cp->m_pc;

		d2d::ISymbol* symbol = static_cast<d2d::ISymbol*>(pc->ud);
		value["components"][i]["filepath"] = 
			d2d::FilenameTools::getRelativePath(dir, symbol->GetFilepath()).ToStdString();

		if (pc->bind_ps_cfg) {
			std::string filepath = PSConfigMgr::Instance()->GetFilepath(pc->bind_ps_cfg);
			value["components"][i]["bind ps filepath"] = 
				d2d::FilenameTools::getRelativePath(dir, filepath).ToStdString();
		}

		value["components"][i]["name"] = cp->m_name->GetValue().ToStdString();
		value["components"][i]["start_z"] = cp->m_startz->GetValue();
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
				  ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	FileAdapter adapter;
	adapter.Load(filepath);

	int version = value["version"].asInt();
	if (version == 0) {
		value["life"]["center"] = adapter.life;
		value["life"]["offset"] = adapter.life_var;
		value["speed"]["center"] = adapter.spd;
		value["speed"]["offset"] = adapter.spd_var;
	}

	toolbar->Load(value, version);

	toolbar->m_name->SetValue(adapter.name);
	toolbar->m_package->SetValue(adapter.package);

	toolbar->m_layer->SetValue(adapter.layer);

	toolbar->m_min_hori->SetValue((adapter.hori - adapter.hori_var) * d2d::TRANS_RAD_TO_DEG);
	toolbar->m_max_hori->SetValue((adapter.hori + adapter.hori_var) * d2d::TRANS_RAD_TO_DEG);
	ps->SetHori(toolbar->m_min_hori->GetValue(), toolbar->m_max_hori->GetValue());

	toolbar->m_min_vert->SetValue((adapter.vert - adapter.vert_var) * d2d::TRANS_RAD_TO_DEG);
	toolbar->m_max_vert->SetValue((adapter.vert + adapter.vert_var) * d2d::TRANS_RAD_TO_DEG);
	ps->SetVert(toolbar->m_min_vert->GetValue(), toolbar->m_max_vert->GetValue());

	toolbar->m_inertia->SetValue(adapter.inertia);
	ps->SetInertia(toolbar->m_inertia->GetValue());

	toolbar->m_bounce->SetValue(adapter.bounce);
	ps->SetBounce(adapter.bounce);

	toolbar->m_additiveBlend->SetValue(adapter.additive_blend);
	ps->SetAdditiveBlend(adapter.additive_blend);

	toolbar->m_orient_to_movement->SetValue(adapter.orient_to_movement);
	ps->SetOrientToMovement(adapter.orient_to_movement);

	toolbar->m_radius_3d->SetValue(adapter.is_start_radius_3d);
	ps->SetRadius3D(adapter.is_start_radius_3d);
	toolbar->m_orient_to_parent->SetValue(adapter.orient_to_parent);

	for (size_t i = 0, n = adapter.components.size(); i < n; ++i) {
		toolbar->Add(adapter.components[i]);
	}
}

ParticleSystem* FileIO::LoadPS(const std::string& filepath)
{
	ps_cfg_3d* cfg = PSConfigMgr::Instance()->GetConfig(filepath);
	return new ParticleSystem(PARTICLE_CAP, cfg);
}

ps_cfg_3d* FileIO::LoadPSConfig(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	FileAdapter adapter;
	adapter.Load(filepath);
	
	int sz = sizeof(ps_cfg_3d) + sizeof(particle_symbol) * MAX_COMPONENTS;
	ps_cfg_3d* cfg = (ps_cfg_3d*) operator new(sz);
	memset(cfg, 0, sz);

	cfg->lifetime = adapter.emission_time;
	
	cfg->emission_time = adapter.emission_time;
	cfg->count = adapter.count;

	cfg->life = adapter.life;
	cfg->life_var = adapter.life_var;

	cfg->hori = adapter.hori;
	cfg->hori_var = adapter.hori_var;
	cfg->vert = adapter.vert;
	cfg->vert_var = adapter.vert_var;

	cfg->spd = adapter.spd;
	cfg->spd_var = adapter.spd_var;
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

	cfg->bounce = adapter.bounce;

	cfg->start_radius = adapter.start_radius;
	cfg->is_start_radius_3d = adapter.is_start_radius_3d;

	cfg->orient_to_movement = adapter.orient_to_movement;

	// todo dir
	cfg->dir.x = 0;
	cfg->dir.y = 0;
	cfg->dir.z = 1;

	cfg->symbol_count = adapter.components.size();
	cfg->symbols = (particle_symbol*)(cfg+1);
	for (int i = 0, n = adapter.components.size(); i < n; ++i) {
		const FileAdapter::Component& comp = adapter.components[i];
		particle_symbol& symbol = cfg->symbols[i];

		symbol.scale_start = comp.scale_start;
		symbol.scale_end = comp.scale_end;

		symbol.angle = comp.angle;
		symbol.angle_var = comp.angle_var;

		memcpy(&symbol.col_mul.r, &comp.col_mul.r, sizeof(comp.col_mul));
		memcpy(&symbol.col_add.r, &comp.col_add.r, sizeof(comp.col_add));
		symbol.alpha_start = comp.alpha_start;
		symbol.alpha_end = comp.alpha_end;

		symbol.bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(comp.bind_filepath);

		symbol.ud = d2d::SymbolMgr::Instance()->FetchSymbol(comp.filepath);
	}

	return cfg;
}

}