#include "FileIO.h"
#include "ParticleSystem.h"
#include "PSConfigMgr.h"
#include "ToolBarPanel.h"
#include "ComponentPanel.h"
#include "ps_config.h"

#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/SliderCtrl.h>
#include <ee/Math2D.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>

#include <ps_3d.h>
#include <sprite2/P3dEmitterCfg.h>
#include <gum/JsonSerializer.h>
#include <gum/P3dSymLoader.h>

#include <wx/spinctrl.h>

#include <fstream>

namespace eparticle3d
{

void FileIO::Store(const std::string& filepath, ParticleSystem* ps,
				   ToolbarPanel* toolbar)
{
	Json::Value value;

	toolbar->Store(value);
	static const int VERSION = 1;
	value["version"] = VERSION;

	value["name"] = ps->name;

	value["static_mode"] = toolbar->m_static_mode->GetValue();

	value["min_hori"] = toolbar->m_min_hori->GetValue();
	value["max_hori"] = toolbar->m_max_hori->GetValue();
	value["min_vert"] = toolbar->m_min_vert->GetValue();
	value["max_vert"] = toolbar->m_max_vert->GetValue();

	value["ground"] = toolbar->m_ground->GetSelection();
	
	const s2::P3dEmitterCfg* cfg = ps->GetConfig();
	value["orient_to_movement"] = cfg->GetImpl()->orient_to_movement;

	value["loop"] = ps->IsLoop();
	value["local"] = ps->IsLocal();

//	value["orient_to_parent"] = toolbar->m_orient_to_parent->GetValue();

	value["blend"] = toolbar->m_blend->GetSelection();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (size_t i = 0, n = toolbar->m_children.size(); i < n; ++i)
	{
		ComponentPanel* cp = toolbar->m_children[i];
		p3d_symbol* pc = cp->m_pc;

		value["components"][i]["count"] = pc->count;

		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(pc->ud));
		value["components"][i]["filepath"] = 
			ee::FileHelper::GetRelativePath(dir, sym->GetFilepath());

		value["components"][i]["name"] = cp->m_name->GetValue().ToStdString();
		for (int j = 0, m = cp->m_sliders.size(); j < m; ++j) {
			cp->m_sliders[j]->Store(value["components"][i]);
		}

		gum::JsonSerializer::Store(pc->mul_col_begin.rgba, value["components"][i]["mul_col_begin"]);
		gum::JsonSerializer::Store(pc->mul_col_end.rgba, value["components"][i]["mul_col_end"]);
		gum::JsonSerializer::Store(pc->add_col_begin.rgba, value["components"][i]["add_col_begin"]);
		gum::JsonSerializer::Store(pc->add_col_end.rgba, value["components"][i]["add_col_end"]);
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

	gum::P3dSymLoader adapter;
	adapter.LoadJson(filepath);
	
	int version = value["version"].asInt();
	if (version == 0) {
		value["life"]["center"] = adapter.life;
		value["life"]["offset"] = adapter.life_var;
	}

	if (value["radial_speed"].isNull()) {
		value["radial_speed"]["center"] = adapter.radial_spd * 4;
		value["radial_speed"]["offset"] = adapter.radial_spd_var * 4;
	}

	ps->SetLoop(adapter.loop);
	ps->SetLocal(adapter.local);

	toolbar->Load(value, version);

	ps->name = adapter.name;

	bool static_mode = false;
	if (!value["static_mode"].isNull()) {
		static_mode = value["static_mode"].asBool();
	}
	toolbar->OnSetStaticMode(static_mode);

	toolbar->m_min_hori->SetValue((adapter.hori - adapter.hori_var) * SM_RAD_TO_DEG);
	toolbar->m_max_hori->SetValue((adapter.hori + adapter.hori_var) * SM_RAD_TO_DEG);
	ps->SetHori(toolbar->m_min_hori->GetValue(), toolbar->m_max_hori->GetValue());

	toolbar->m_min_vert->SetValue((adapter.vert - adapter.vert_var) * SM_RAD_TO_DEG);
	toolbar->m_max_vert->SetValue((adapter.vert + adapter.vert_var) * SM_RAD_TO_DEG);
	ps->SetVert(toolbar->m_min_vert->GetValue(), toolbar->m_max_vert->GetValue());

	toolbar->m_ground->SetSelection(adapter.ground);
	ps->SetGround(adapter.ground);

	toolbar->m_orient_to_movement->SetValue(adapter.orient_to_movement);
	ps->SetOrientToMovement(adapter.orient_to_movement);

//	toolbar->m_orient_to_parent->SetValue(adapter.orient_to_parent);

	toolbar->m_blend->SetSelection(adapter.blend);
	ps->SetBlend(adapter.blend);

	for (size_t i = 0, n = adapter.components.size(); i < n; ++i) {
		toolbar->Add(adapter.components[i], library);
	}
}

ParticleSystem* FileIO::LoadPS(const std::string& filepath)
{
	s2::P3dEmitterCfg* cfg = PSConfigMgr::Instance()->GetConfig(filepath);
	return new ParticleSystem(cfg, false);
}

s2::P3dEmitterCfg* FileIO::LoadPSConfig(const std::string& filepath)
{
	class Loader : public gum::P3dSymLoader
	{
	protected:
		virtual s2::Symbol* LoadSymbol(const std::string& filepath) const {
			return ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		}
	}; // Loader

	Loader adapter;
	adapter.LoadJson(filepath);

 	int sz = SIZEOF_P3D_EMITTER_CFG + SIZEOF_P3D_SYMBOL * MAX_COMPONENTS;
 	p3d_emitter_cfg* cfg = (p3d_emitter_cfg*) operator new(sz);
 	memset(cfg, 0, sz);
	adapter.Store(cfg);

	return new s2::P3dEmitterCfg(cfg);
}

}