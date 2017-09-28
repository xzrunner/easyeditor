#include "Symbol.h"

#include <ee/StringHelper.h>
#include <ee/SpriteLoader.h>
#include <ee/SymbolFile.h>
#include <ee/SettingData.h>
#include <ee/Config.h>

#include <sprite2/SymType.h>
#include <gum/StringHelper.h>
#include <gum/FilepathHelper.h>

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = ee::SymbolFile::Instance()->Tag(s2::SYM_SCALE9) + gum::StringHelper::ToString(id++);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	setting.open_image_edge_clip = false;

	ee::SpriteLoader spr_loader;
	gum::Scale9SymLoader loader(this, &spr_loader);
	loader.LoadJson(m_filepath);

	setting.open_image_edge_clip = true;

	return true;
}

}