#include "FileIO.h"
#include "ToolBarPanel.h"
#include "ComponentPanel.h"
#include "ParticleSystem.h"
#include "PSConfigMgr.h"
#include "ps_config.h"

#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/Math2D.h>
#include <ee/SymbolMgr.h>

#include <ps_2d.h>
#include <glue/JsonSerializer.h>

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

		glue::JsonSerializer::Store(p_symbol->mul_col_begin.rgba, cval["mul_col_begin"]);
		glue::JsonSerializer::Store(p_symbol->mul_col_end.rgba, cval["mul_col_end"]);
		glue::JsonSerializer::Store(p_symbol->add_col_begin.rgba, cval["add_col_begin"]);
		glue::JsonSerializer::Store(p_symbol->add_col_end.rgba, cval["add_col_end"]);

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

	glue::P2dSymLoader adapter;
	adapter.LoadJson(filepath);

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
	class Loader : public glue::P2dSymLoader
	{
	protected:
		virtual s2::Symbol* LoadSymbol(const std::string& filepath) const {
			return ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		}
	}; // Loader

	Loader adapter;
	adapter.LoadJson(filepath);

	int sz = SIZEOF_P2D_EMITTER_CFG + SIZEOF_P2D_SYMBOL * MAX_COMPONENTS;
	p2d_emitter_cfg* cfg = (p2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);

	adapter.Store(cfg);

	return cfg;
}

}