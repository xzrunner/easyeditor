#include "FileIO.h"
#include "UIList.h"

#include <easycomplex.h>

namespace eui
{
namespace list
{

void FileIO::Store(const char* filename, const UIList* list)
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	// items complex
	ecomplex::Symbol items_complex;
	list->items.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(items_complex.m_sprites), d2d::DT_ALL);
	items_complex.m_clipbox = list->clipbox;
	std::string items_path = name + "_items_complex.json";
	ecomplex::FileStorer::Store(items_path.c_str(), &items_complex);

	// top complex
	ecomplex::Sprite items_sprite(&items_complex);
	ecomplex::Symbol top_complex;
	list->bgs.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(top_complex.m_sprites), d2d::DT_ALL);
	items_sprite.Retain();
	top_complex.m_sprites.push_back(&items_sprite);
	std::string top_path = name + "_complex.json";
	ecomplex::FileStorer::Store(top_path.c_str(), &top_complex);

	// ui
	std::string ui_path = name + "_listui.json";
	Json::Value value;
	value["type"] = "list";
	value["vertical"] = list->vertical;
	assert(items_complex.m_sprites.size() >= 2);
	if (list->vertical) {
		value["item space"] = fabs(items_complex.m_sprites[0]->GetPosition().y - items_complex.m_sprites[1]->GetPosition().y);
	} else {
		value["item space"] = fabs(items_complex.m_sprites[0]->GetPosition().x - items_complex.m_sprites[1]->GetPosition().x);
	}
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(ui_path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

}
}