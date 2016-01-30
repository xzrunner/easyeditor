#include "Code.h"

#include <queue>

#include <easycomplex.h>
#include <easybuilder.h>

namespace eui
{
namespace window
{

namespace lua = ebuilder::lua;

Code::Code(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
}

struct Node
{
	const d2d::Sprite* sprite;
	std::string path;
	std::string name;

	Node(const d2d::Sprite* s, const std::string& p) 
		: sprite(s), path(p) {}

}; // Node

void Code::ResolveUI(const std::vector<d2d::Sprite*>& sprites)
{
	ebuilder::CodeGenerator *gen_init = new ebuilder::CodeGenerator(), 
		*gen_path = new ebuilder::CodeGenerator();
	std::vector<std::pair<std::string, std::string> > text_nodes;
	{
		lua::Function func(*gen_path, "ui:_init_path", 0);

		gen_path->line("local path = {}");

		std::queue<Node> buffer;
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			Node node(sprites[i], "");
			buffer.push(node);
		}

		while (!buffer.empty()) 
		{
			Node parent = buffer.front(); buffer.pop();
			const d2d::Sprite* spr = parent.sprite;

			if (spr->name.empty() || spr->name[0] == '_') {
				continue;
			}

			std::string path, name;
			if (parent.path.empty()) {
				path = spr->name;
				name = spr->name;
			} else {
				path = parent.path + "." + spr->name;
				name = parent.name + "_" + spr->name;
			}

			gen_path->line(lua::assign("path."+name, "\""+path+"\""));

			if (const d2d::FontBlankSprite* s = dynamic_cast<const d2d::FontBlankSprite*>(spr))
			{
				text_nodes.push_back(std::make_pair("path."+name, "i18n."+s->GetTextID()));
			}
			else if (const ecomplex::Sprite* e_spr = dynamic_cast<const ecomplex::Sprite*>(spr)) 
			{
				for (int i = 0, n = e_spr->GetSymbol().m_sprites.size(); i < n; ++i) {
					Node node(e_spr->GetSymbol().m_sprites[i], "");
					buffer.push(node);
				}
			}
		}

		gen_path->line("self.v_path = path");
	}
	{
		lua::Function func(*gen_init, "ui:init", 0);
		gen_init->line("local spr = self.v_uis_spr");
		gen_init->line("local path = self.v_path");
		gen_init->line();
		for (int i = 0, n = text_nodes.size(); i < n; ++i)
		{
			const std::pair<std::string, std::string>& text = text_nodes[i];
			gen_init->line(lua::assign(text.first, text.second));
		}
	}

	m_gen.block(*gen_init);
	m_gen.line();
	m_gen.block(*gen_path);
}

void Code::ResolveText(const std::vector<d2d::Sprite*>& sprites)
{
	std::queue<d2d::Sprite*> buffer;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		buffer.push(sprites[i]);
	}

 	while (!buffer.empty()) 
 	{
		d2d::Sprite* spr = buffer.front(); buffer.pop();

		if (spr->name.empty() || spr->name[0] == '_') {
			continue;
		}

		if (d2d::FontBlankSprite* s = dynamic_cast<d2d::FontBlankSprite*>(spr))
		{
			std::string content = s->GetTextContext().ToStdString();
			size_t pos = 0;
			std::string replace("\\n");
			while ((pos = content.find('\n', pos)) != std::string::npos) {
				content.replace(pos, 1, replace);
				pos += replace.length();
			}

			m_gen.line(lua::assign("[\""+s->GetTextID()+"\"]", "\""+content+"\","));
		}
		else if (ecomplex::Sprite* e_spr = dynamic_cast<ecomplex::Sprite*>(spr)) 
		{
			for (int i = 0, n = e_spr->GetSymbol().m_sprites.size(); i < n; ++i) {
				buffer.push(e_spr->GetSymbol().m_sprites[i]);
			}
		}
 	}
}

}
}