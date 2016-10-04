#include "TransOldAnchorFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/FontBlankSprite.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>
#include <easyanim.h>

#include <sprite2/S2_Sprite.h>
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
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ecomplex::Symbol* complex = static_cast<ecomplex::Symbol*>(sym);
	bool dirty = false;
	const std::vector<s2::Sprite*>& children = complex->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);
		if (IsAnchor(spr)) {
			spr->SetAnchor(true);
			dirty = true;
		}
	}
	if (dirty) {
		ecomplex::FileStorer::Store(filepath.c_str(), complex);
	}
	sym->RemoveReference();
}

void TransOldAnchorFile::TransAnimation(const std::string& filepath) const
{
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	eanim::Symbol* anim = static_cast<eanim::Symbol*>(sym);
	bool dirty = false;
	const std::vector<s2::AnimSymbol::Layer*>& layers = anim->GetLayers();
	for (int i = 0, n = layers.size(); i < n; ++i) {
		s2::AnimSymbol::Layer* layer = layers[i];
		for (int j = 0, m = layer->frames.size(); j < m; ++j) {
			s2::AnimSymbol::Frame* frame = layer->frames[j];
			for (int k = 0, l = frame->sprs.size(); k < l; ++k) {
				ee::Sprite* spr = dynamic_cast<ee::Sprite*>(frame->sprs[k]);
				if (IsAnchor(spr)) {
					spr->SetAnchor(true);
					dirty = true;
				}
			}
		}
	}
	if (dirty) {
		eanim::FileSaver::Store(filepath, *anim);
	}
	sym->RemoveReference();
}

bool TransOldAnchorFile::IsAnchor(const ee::Sprite* spr) const
{	
	if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr)) {
		return font->font.empty() && font->font_color == s2::Color(0, 0, 0, 0);
	} else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		const std::vector<s2::Sprite*>& children = dynamic_cast<const s2::ComplexSymbol*>(complex->GetSymbol())->GetChildren();
		if (children.size() == 1) {
			ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[0]);
			return IsAnchor(child);
		} else {
			return false;
		}
	}
	return false;
}

}