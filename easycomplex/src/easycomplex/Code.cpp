#include "Code.h"

#include <queue>

#include <easycomplex.h>
#include <easybuilder.h>

namespace ecomplex
{

namespace lua = ebuilder::lua;

Code::Code(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
}

struct Node
{
	const Symbol& symbol;
	std::string path;
	std::string name;

	Node(const Symbol& _symbol, const std::string& _path) 
		: symbol(_symbol), path(_path) {}
}; // Node

void Code::ResolveUI(const Symbol& symbol)
{
	ebuilder::CodeGenerator *gen_init = new ebuilder::CodeGenerator(), 
		*gen_path = new ebuilder::CodeGenerator();
	std::vector<std::pair<std::string, std::string> > text_nodes;
	{
		lua::Function func(*gen_path, "ui:_init_path", 0);

		gen_path->line("local path = {}");

		std::queue<Node> buffer;
		Node root(symbol, "");

		buffer.push(root);
		while (!buffer.empty()) 
		{
			Node parent = buffer.front(); buffer.pop();
			const Symbol& parent_symbol = parent.symbol;
			for (int i = 0, n = parent_symbol.m_sprites.size(); i < n; ++i)
			{
				d2d::ISprite* child = parent_symbol.m_sprites[i];
				if (child->name.empty() || child->name[0] == '_') {
					continue;
				}

				std::string path, name;
				if (parent.path.empty()) {
					path = child->name;
					name = child->name;
				} else {
					path = parent.path + "." + child->name;
					name = parent.name + "_" + child->name;
				}

				gen_path->line(lua::assign("path."+name, "\""+path+"\""));

				if (Sprite* s = dynamic_cast<Sprite*>(child)) 
				{
					Node next(s->GetSymbol(), path);
					next.name = name;
					buffer.push(next);
				} 
				else if (d2d::FontSprite* s = dynamic_cast<d2d::FontSprite*>(child))
				{
					text_nodes.push_back(std::make_pair("path."+name, "i18n."+s->GetTextID()));
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

void Code::ResolveText(const Symbol& symbol)
{
	std::queue<const Symbol*> buffer;
	buffer.push(&symbol);
 	while (!buffer.empty()) 
 	{
 		const Symbol* parent = buffer.front(); buffer.pop();
 		for (int i = 0, n = parent->m_sprites.size(); i < n; ++i)
 		{
 			d2d::ISprite* child = parent->m_sprites[i];
 			if (child->name.empty() || child->name[0] == '_') {
 				continue;
 			}
 
 			if (d2d::FontSprite* s = dynamic_cast<d2d::FontSprite*>(child))
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
 		}
 	}
}

}