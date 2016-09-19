#include "MaskSymLoader.h"
#include "FilepathHelper.h"
#include "SpriteFactory.h"

#include <sprite2/MaskSymbol.h>
#include <sprite2/S2_Sprite.h>
#include <simp/NodeMask.h>

#include <json/json.h>

#include <fstream>

namespace gum
{

MaskSymLoader::MaskSymLoader(s2::MaskSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

MaskSymLoader::~MaskSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void MaskSymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value.isMember("base") || !value.isMember("mask")) {
		return;
	}

	std::string dir = FilepathHelper::Dir(filepath);

	s2::MaskSymbol* psym = VI_DOWNCASTING<s2::MaskSymbol*>(m_sym);

	std::string base_path = FilepathHelper::Absolute(dir, value["base"]["filepath"].asString());
	s2::Sprite* base = SpriteFactory::Instance()->Create(base_path);
	psym->SetBase(base);
	base->RemoveReference();

	std::string mask_path = FilepathHelper::Absolute(dir, value["mask"]["filepath"].asString());
	s2::Sprite* mask = SpriteFactory::Instance()->Create(mask_path);
	psym->SetMask(mask);
	mask->RemoveReference();
}

void MaskSymLoader::LoadBin(const simp::NodeMask* node)
{
	if (!m_sym) {
		return;
	}

	s2::MaskSymbol* psym = VI_DOWNCASTING<s2::MaskSymbol*>(m_sym);

	s2::Sprite* base = SpriteFactory::Instance()->Create(node->base_id);
	psym->SetBase(base);
	base->RemoveReference();

	s2::Sprite* mask = SpriteFactory::Instance()->Create(node->mask_id);
	psym->SetMask(mask);
	mask->RemoveReference();
}

}