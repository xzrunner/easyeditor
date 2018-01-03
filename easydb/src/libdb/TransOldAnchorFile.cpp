#include "TransOldAnchorFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/FontBlankSprite.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>
#include <easyanim.h>

#include <sprite2/Sprite.h>
#include <sprite2/SymType.h>

namespace edb
{

std::string TransOldAnchorFile::Command() const
{
	return "trans-old-anchor";
}

std::string TransOldAnchorFile::Description() const
{
	return "trans old anchor file";
}

std::string TransOldAnchorFile::Usage() const
{
	// trans-old-anchor dir
	std::string usage = Command() + " [folder]";
	return usage;
}

int TransOldAnchorFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void TransOldAnchorFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			TransComplex(filepath);
			break;
		case s2::SYM_ANIMATION:
			TransAnimation(filepath);
			break;
		}
	}
}

void TransOldAnchorFile::TransComplex(const std::string& filepath) const
{
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	auto complex = std::dynamic_pointer_cast<ecomplex::Symbol>(sym);
	bool dirty = false;
	auto& children = complex->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto spr = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		if (IsAnchor(spr)) {
			spr->SetAnchor(true);
			dirty = true;
		}
	}
	if (dirty) {
		ecomplex::FileStorer::Store(filepath, *complex, ee::FileHelper::GetFileDir(filepath), false);
	}
}

void TransOldAnchorFile::TransAnimation(const std::string& filepath) const
{
	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	auto anim = std::dynamic_pointer_cast<libanim::Symbol>(sym);
	bool dirty = false;
	auto& layers = anim->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		auto& layer = layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			auto& frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				auto& spr = std::dynamic_pointer_cast<ee::Sprite>(frame->sprs[k]);
				if (IsAnchor(spr)) {
					spr->SetAnchor(true);
					dirty = true;
				}
			}
		}
	}
	if (dirty) {
		libanim::FileSaver::Store(filepath, *anim);
	}
}

bool TransOldAnchorFile::IsAnchor(const ee::SprConstPtr& spr) const
{	
	if (auto font = std::dynamic_pointer_cast<const ee::FontBlankSprite>(spr)) {
		return font->font.empty() && font->font_color == s2::Color(0, 0, 0, 0);
	} else if (auto complex = std::dynamic_pointer_cast<const ecomplex::Sprite>(spr)) {
		auto& children = std::dynamic_pointer_cast<s2::ComplexSymbol>(complex->GetSymbol())->GetAllChildren();
		if (children.size() == 1) {
			auto child = std::dynamic_pointer_cast<ee::Sprite>(children[0]);
			return IsAnchor(child);
		} else {
			return false;
		}
	}
	return false;
}

}