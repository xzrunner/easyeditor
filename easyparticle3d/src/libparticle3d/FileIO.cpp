#include "FileIO.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ParticleSystem.h"
#include "config.h"
#include "ps_config.h"

namespace eparticle3d
{

void FileIO::Store(const char* filepath, ToolbarPanel* toolbar)
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

		value["components"][i]["filepath"] = 
			d2d::FilenameTools::getRelativePath(dir, pc->symbol->GetFilepath()).ToStdString();
		if (pc->bind_ps) {
			value["components"][i]["bind ps filepath"] = 
				d2d::FilenameTools::getRelativePath(dir, pc->bind_ps->m_filepath).ToStdString();
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
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, ParticleSystem* ps,
				  ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int version = value["version"].asInt();

	FileAdapter adapter;
	adapter.load(filepath, version);

	if (version == 0) {
		int center = (adapter.min_life + adapter.max_life) * 0.5f;
		int offset = (adapter.max_life - adapter.min_life) * 0.5f;
		value["life"]["center"] = center;
		value["life"]["offset"] = offset;

		center = (adapter.min_spd + adapter.max_spd) * 0.5f;
		offset = (adapter.max_spd - adapter.min_spd) * 0.5f;
		value["speed"]["center"] = center;
		value["speed"]["offset"] = offset;
	}

	toolbar->Load(value, version);

	//////////////////////////////////////////////////////////////////////////

	toolbar->m_name->SetValue(adapter.name);
	toolbar->m_package->SetValue(adapter.package);

	toolbar->m_layer->SetValue(adapter.layer);
	toolbar->m_min_hori->SetValue(adapter.min_hori);
	toolbar->m_max_hori->SetValue(adapter.max_hori);
	ps->SetHori(adapter.min_hori, adapter.max_hori);
	toolbar->m_min_vert->SetValue(adapter.min_vert);
	toolbar->m_max_vert->SetValue(adapter.max_vert);
	ps->SetVert(adapter.min_vert, adapter.max_vert);
	toolbar->m_inertia->SetValue(adapter.inertia);
	toolbar->m_bounce->SetValue(adapter.bounce);
	ps->SetBounce(adapter.bounce);
	toolbar->m_additiveBlend->SetValue(adapter.additive_blend);
	ps->SetAdditiveBlend(adapter.additive_blend);
	toolbar->m_orient_to_movement->SetValue(adapter.orient_to_movement);
	ps->SetOrientToMovement(adapter.orient_to_movement);
	toolbar->m_radius_3d->SetValue(adapter.start_radius_3d);
	ps->SetRadius3D(adapter.start_radius_3d);
	toolbar->m_orient_to_parent->SetValue(adapter.orient_to_parent);
	for (size_t i = 0, n = adapter.children.size(); i < n; ++i) {
		toolbar->Add(adapter.children[i]);
	}
}

ParticleSystem* FileIO::LoadPS(const char* filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int version = value["version"].asInt();

	FileAdapter adapter;
	adapter.load(filepath, version);

	ParticleSystem* ps = new ParticleSystem(PARTICLE_CAP);

	ps->m_filepath = filepath;
	ps->SetValue(PS_COUNT, d2d::UICallback::Data(adapter.count));
	ps->SetValue(PS_EMISSION_TIME, d2d::UICallback::Data(adapter.emission_time));
	ps->SetValue(PS_LIFE_TIME, d2d::UICallback::Data((adapter.min_life + adapter.max_life) * 0.5f, (adapter.max_life - adapter.min_life) * 0.5f));
	ps->SetValue(PS_SPEED, d2d::UICallback::Data((adapter.min_spd + adapter.max_spd) * 0.5f, (adapter.max_spd - adapter.min_spd) * 0.5f));
	ps->SetValue(PS_ANGULAR_SPEED, d2d::UICallback::Data((adapter.min_angular_spd + adapter.max_angular_spd) * 0.5f, (adapter.max_angular_spd - adapter.min_angular_spd) * 0.5f));
	ps->SetValue(PS_DISTURBANCE_RADIUS, d2d::UICallback::Data((adapter.min_dis_region + adapter.max_dis_region) * 0.5f, (adapter.max_dis_region - adapter.min_dis_region) * 0.5f));
	ps->SetValue(PS_DISTURBANCE_SPD, d2d::UICallback::Data((adapter.min_dis_spd + adapter.max_dis_spd) * 0.5f, (adapter.max_dis_spd - adapter.min_dis_spd) * 0.5f));
	ps->SetValue(PS_GRAVITY, d2d::UICallback::Data(adapter.gravity));
	ps->SetValue(PS_LINEAR_ACC, d2d::UICallback::Data((adapter.min_linear_acc + adapter.max_linear_acc) * 0.5f, (adapter.max_linear_acc - adapter.min_linear_acc) * 0.5f));
	ps->SetValue(PS_FADEOUT_TIME, d2d::UICallback::Data(adapter.fadeout_time));
	ps->SetValue(PS_START_RADIUS, d2d::UICallback::Data(adapter.start_radius));

	ps->SetHori(adapter.min_hori, adapter.max_hori);
	ps->SetVert(adapter.min_vert, adapter.max_vert);
	ps->SetBounce(adapter.bounce);
	ps->SetAdditiveBlend(adapter.additive_blend);
	ps->SetRadius3D(adapter.start_radius_3d);
	ps->SetOrientToMovement(adapter.orient_to_movement);

	for (size_t i = 0, n = adapter.children.size(); i < n; ++i)
	{
		const FileAdapter::Child& child = adapter.children[i];

		// todo Release symbol
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(child.filepath);

		particle_symbol* pc = new particle_symbol;

		pc->scale_start = child.start_scale * 0.01f;
		pc->scale_end = child.end_scale * 0.01f;
		pc->angle = (child.min_rotate + child.max_rotate) * 0.5f * d2d::TRANS_DEG_TO_RAD;
		pc->angle_var = (child.max_rotate - child.min_rotate) * 0.5f * d2d::TRANS_DEG_TO_RAD;
		memcpy(&pc->col_mul.r, &child.mul_col.r, sizeof(pc->col_mul));
		memcpy(&pc->col_add.r, &child.add_col.r, sizeof(pc->col_add));
		pc->alpha_start = child.start_alpha * 0.01f;
		pc->alpha_end = child.end_alpha * 0.01f;

		ps->m_ps->cfg.symbols[ps->m_ps->cfg++] = pc;
	}

	return ps;
}

ps_cfg_3d* FileIO::LoadPSConfig(const char* filepath)
{

}

}